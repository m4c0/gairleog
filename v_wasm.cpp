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

struct app_stuff : v::base_app_stuff {};
// TODO: fix wasm bug when resizing window
struct ext_stuff {};

namespace v {
  static int g_program;
  static int g_u_aspect;
  static int g_u_uni;
  static bool g_loaded = false;

  void setup() {
    using namespace gelo;

    auto p = g_program = create_program();
    attach_shader(p, vv::as()->vert.id());
    attach_shader(p, vv::as()->frag.id());

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

    vv::as()->buffer.bind();

    auto attrs = vv::as()->vertex_attributes();
    for (auto i = 0; i < attrs.size(); i++) attrs[i](i);

    g_loaded = true;
  }

  void render() {
    using namespace gelo;

    uniform1f(g_u_aspect, casein::window_size.x / casein::window_size.y);

    clear_color(0, 0, 0, 1);
    clear(COLOR_BUFFER_BIT);
    viewport(0, 0, casein::window_size.x, casein::window_size.y);
    draw_arrays_instanced(TRIANGLE_STRIP, 0, 4, vv::as()->buffer.count());
  }
}

struct mapper : v::mapper {
  decltype(vv::as()->buffer.map()) m = vv::as()->buffer.map();

  void add_sprite(v::sprite s) {
    s.grid_size.y *= -1;
    m += s;
  }
};

hai::uptr<v::mapper> v::map() {
  return hai::uptr<v::mapper> { new ::mapper {} };
}

static void on_frame() {
  if (!*vv::as()) return;
  if (!v::g_program) v::setup();
  if (!v::g_loaded) return; // Last resource to load
  v::call_on_frame();
  v::render();
}

const int i = [] {
  vv::setup(on_frame);
  return 0;
}();
