module v;
import casein;
import gelo;
import jute;
import silog;
import sires;
import stubby;
import traits;
import vaselin;

using namespace jute::literals;

static hai::varray<v::sprite> buffer { 10240 };

namespace v {
  static constexpr const float quad[] { 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0 };

  static hai::cstr vert_shader {};
  static hai::cstr frag_shader {};

  static void shader(int prog, int type, jute::view src) {
    using namespace gelo;
  
    auto v = create_shader(type);
    shader_source(v, src.begin(), src.size());
    compile_shader(v);
    if (!get_shader_parameter_b(v, COMPILE_STATUS)) {
      char buf[1024] {};
      get_shader_info_log(v, buf, sizeof(buf) - 1);
      silog::log(silog::error, "Error compiling shader:\n%s", buf);
    }
    attach_shader(prog, v);
  }

  static int g_program;
  static int g_quad_buffer;
  static int g_inst_buffer;
  static int g_texture;
  static int g_u_grid_pos;
  static int g_u_grid_size;

  void frame(void *);

  void setup() {
    using namespace gelo;

    auto p = g_program = create_program();
    shader(p, VERTEX_SHADER, vert_shader);
    shader(p, FRAGMENT_SHADER, frag_shader);

    link_program(p);
    if (!get_program_parameter_b(p, LINK_STATUS)) {
      char buf[1024] {};
      get_program_info_log(p, buf, sizeof(buf) - 1);
      silog::log(silog::error, "Error linking program:\n%s", buf);
    }

    use_program(p);

    enable(BLEND);
    blend_func(ONE, ONE_MINUS_SRC_ALPHA);

    g_u_grid_pos = get_uniform_location(p, "_18.grid_pos");
    g_u_grid_size = get_uniform_location(p, "_18.grid_size");
    auto u_tex = get_uniform_location(p, "tex");
    uniform1i(u_tex, 0); 

    auto b = g_quad_buffer = create_buffer();
    bind_buffer(ARRAY_BUFFER, b);
    buffer_data(ARRAY_BUFFER, quad, sizeof(quad), STATIC_DRAW);
    enable_vertex_attrib_array(0);
    vertex_attrib_pointer(0, 2, FLOAT, false, 0, 0);

    static constexpr const auto stride = sizeof(sprite);

    b = g_inst_buffer = gelo::create_buffer();
    bind_buffer(ARRAY_BUFFER, b);

    enable_vertex_attrib_array(1);
    vertex_attrib_pointer(1, 2, FLOAT, false, stride, traits::offset_of(&v::sprite::pos));
    vertex_attrib_divisor(1, 1);

    enable_vertex_attrib_array(2);
    vertex_attrib_i_pointer(2, 1, UNSIGNED_INT, stride, traits::offset_of(&v::sprite::id));
    vertex_attrib_divisor(2, 1);

    enable_vertex_attrib_array(3);
    vertex_attrib_pointer(3, 1, FLOAT, false, stride, traits::offset_of(&v::sprite::mult));
    vertex_attrib_divisor(3, 1);
  }

  void load_texture(jute::view name) {
    using namespace gelo;

    unsigned pix = 0xFF00FFFF;

    auto t = g_texture = gelo::create_texture();
    active_texture(TEXTURE0);
    bind_texture(TEXTURE_2D, t);
    tex_image_2d(TEXTURE_2D, 0, RGBA, 1, 1, 0, RGBA, UNSIGNED_BYTE, &pix, sizeof(pix));

    sires::read(name, nullptr, [name](auto, hai::cstr & bits) {
      using namespace gelo;

      silog::log(silog::info, "[%*s] loaded", static_cast<int>(name.size()), name.begin());

      auto img = stbi::load(bits);
      auto & [ w, h, ch, data ] = img;
      bind_texture(TEXTURE_2D, g_texture);
      tex_image_2d(TEXTURE_2D, 0, RGBA, w, h, 0, RGBA, UNSIGNED_BYTE, *data, w * h * 4);
      tex_parameter_i(TEXTURE_2D, TEXTURE_WRAP_S, CLAMP_TO_EDGE);
      tex_parameter_i(TEXTURE_2D, TEXTURE_WRAP_T, CLAMP_TO_EDGE);
      tex_parameter_i(TEXTURE_2D, TEXTURE_MIN_FILTER, NEAREST);
      tex_parameter_i(TEXTURE_2D, TEXTURE_MAG_FILTER, NEAREST);

      // Last resource loaded, let's start
      vaselin::request_animation_frame(frame, nullptr);
    });
  }

  void create_window() {
    sires::read("gairleog.vert.gles", nullptr, [](auto, hai::cstr & gles) {
      vert_shader = traits::move(gles);

      // TODO: load in parallel
      sires::read("gairleog.frag.gles", nullptr, [](auto, hai::cstr & gles) {
        frag_shader = traits::move(gles);

        setup();
        load_texture("pixelite2.png");
      });
    });
  }

  void render() {
    using namespace gelo;

    float aspect = casein::window_size.x / casein::window_size.y;

    clear_color(0, 0, 0, 1);
    clear(COLOR_BUFFER_BIT);
    uniform2f(g_u_grid_pos, v::pc.grid_pos.x, v::pc.grid_pos.y);
    uniform2f(g_u_grid_size, v::pc.grid_size.x * aspect, v::pc.grid_size.y);
    viewport(0, 0, casein::window_size.x, casein::window_size.y);

    bind_buffer(ARRAY_BUFFER, g_inst_buffer);
    buffer_data(ARRAY_BUFFER, buffer.begin(), buffer.size() * sizeof(sprite), DYNAMIC_DRAW);
    draw_arrays_instanced(TRIANGLES, 0, 6, buffer.size());
  }
}

struct mapper : v::mapper {
  mapper() {
    buffer.truncate(0);
  }
  void push(v::sprite s) override {
    buffer.push_back(s);
  }
};

hai::uptr<v::mapper> v::map() {
  return hai::uptr<v::mapper> { new ::mapper {} };
}

void v::frame(void *) {
  v::on_frame();
  v::render();
  vaselin::request_animation_frame(frame, nullptr);
}

const int i = [] {
  using namespace casein;

  handle(CREATE_WINDOW, v::create_window);
  return 0;
}();

