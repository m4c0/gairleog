module v;
import :tools;
import casein;
import gelo;
import jute;
import silog;
import sires;
import traits;
import vinyl;

using namespace jute::literals;

static hai::varray<v::sprite> buffer { 10240 };

namespace v {
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
  static int g_inst_buffer;
  static int g_u_aspect;
  static int g_u_uni;
  static bool g_loaded = false;

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

    g_u_aspect = get_uniform_location(p, "_25.aspect");
    g_u_uni = get_uniform_block_index(p, "uni");
    auto u_tex = get_uniform_location(p, "tex");
    uniform1i(u_tex, 0); 

    static constexpr const auto stride = sizeof(sprite);

    auto b = g_inst_buffer = gelo::create_buffer();
    bind_buffer(ARRAY_BUFFER, b);

    enable_vertex_attrib_array(0);
    vertex_attrib_pointer(0, 2, FLOAT, false, stride, traits::offset_of(&v::sprite::pos));
    vertex_attrib_divisor(0, 1);

    enable_vertex_attrib_array(1);
    vertex_attrib_pointer(1, 2, FLOAT, false, stride, traits::offset_of(&v::sprite::scale));
    vertex_attrib_divisor(1, 1);

    enable_vertex_attrib_array(2);
    vertex_attrib_pointer(2, 4, FLOAT, false, stride, traits::offset_of(&v::sprite::mult));
    vertex_attrib_divisor(2, 1);

    enable_vertex_attrib_array(3);
    vertex_attrib_pointer(3, 2, FLOAT, false, stride, traits::offset_of(&v::sprite::grid_pos));
    vertex_attrib_divisor(3, 1);

    enable_vertex_attrib_array(4);
    vertex_attrib_pointer(4, 2, FLOAT, false, stride, traits::offset_of(&v::sprite::grid_size));
    vertex_attrib_divisor(4, 1);

    enable_vertex_attrib_array(5);
    vertex_attrib_i_pointer(5, 1, UNSIGNED_INT, stride, traits::offset_of(&v::sprite::id));
    vertex_attrib_divisor(5, 1);
  }

  void create_window() {
    sires::read("gairleog.vert.gles", nullptr, [](auto, hai::cstr & gles) {
      vert_shader = traits::move(gles);

      // TODO: load in parallel
      sires::read("gairleog.frag.gles", nullptr, [](auto, hai::cstr & gles) {
        frag_shader = traits::move(gles);

        setup();
        g_loaded = true;
      });
    });
  }

  void render() {
    using namespace gelo;

    uniform1f(g_u_aspect, casein::window_size.x / casein::window_size.y);

    clear_color(0, 0, 0, 1);
    clear(COLOR_BUFFER_BIT);
    viewport(0, 0, casein::window_size.x, casein::window_size.y);

    bind_buffer(ARRAY_BUFFER, g_inst_buffer);
    buffer_data(ARRAY_BUFFER, buffer.begin(), buffer.size() * sizeof(sprite), DYNAMIC_DRAW);
    draw_arrays_instanced(TRIANGLE_STRIP, 0, 4, buffer.size());
  }

}

struct mapper : v::mapper {
  mapper() {
    buffer.truncate(0);
  }
  void add_sprite(v::sprite s) {
    s.grid_size.y *= -1;
    buffer.push_back(s);
  }
};

hai::uptr<v::mapper> v::map() {
  return hai::uptr<v::mapper> { new ::mapper {} };
}

struct app_stuff {
  vinyl::nearest_texture txt { "pixelite2.png" };

  app_stuff() { v::create_window(); };
};
// TODO: fix wasm bug when resizing window
struct ext_stuff {};

static void on_frame() {
  if (!vv::as()->txt) return; // Last resource to load
  if (!v::g_loaded) return; // Last resource to load
  v::call_on_frame();
  v::render();
}

const int i = [] {
  vv::setup(on_frame);
  return 0;
}();
