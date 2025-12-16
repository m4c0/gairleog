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
  class shader {
    int m_id = 0;

  protected:
    shader() = default;
    shader(unsigned type, sv name, sv ext) {
      auto filename = jute::fmt<"%s.%s.gles">(name, ext);
      sires::read(filename, nullptr, [=](auto, hai::cstr & gles) {
        using namespace gelo;

        auto v = gelo::create_shader(type);
        shader_source(v, gles.begin(), gles.size());
        compile_shader(v);
        if (!get_shader_parameter_b(v, COMPILE_STATUS)) {
          char buf[1024] {};
          get_shader_info_log(v, buf, sizeof(buf) - 1);
          silog::log(silog::error, "Error compiling shader:\n%s", buf);
        }
      });
    }

  public:
    constexpr auto id() const { return m_id; }
    explicit constexpr operator bool() const { return m_id; }
  };
  struct vert_shader : shader {
    vert_shader() = default;
    explicit vert_shader(sv name) : shader { gelo::VERTEX_SHADER, name, "vert" } {}
  };
  struct frag_shader : shader {
    frag_shader() = default;
    explicit frag_shader(sv name) : shader { gelo::FRAGMENT_SHADER, name, "frag" } {}
  };

  static vert_shader g_vert_shader {};
  static frag_shader g_frag_shader {};

  static int g_program;
  static int g_inst_buffer;
  static int g_u_aspect;
  static int g_u_uni;
  static bool g_loaded = false;

  void setup() {
    using namespace gelo;

    if (!g_vert_shader || !g_frag_shader) return;

    auto p = g_program = create_program();
    attach_shader(p, g_vert_shader.id());
    attach_shader(p, g_frag_shader.id());

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

    g_loaded = true;
  }

  void create_window() {
    g_vert_shader = vert_shader { "gairleog" };
    g_frag_shader = frag_shader { "gairleog" };
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
