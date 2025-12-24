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
  static int g_u_aspect;

  void render() {
    using namespace gelo;

    uniform1f(g_u_aspect, casein::window_size.x / casein::window_size.y);

    clear_color(0, 0, 0, 1);
    clear(COLOR_BUFFER_BIT);
    viewport(0, 0, casein::window_size.x, casein::window_size.y);
    vv::as()->ppl.cmd_draw();
  }
}

struct mapper : v::mapper {
  decltype(vv::as()->ppl.map()) m = vv::as()->ppl.map();

  void add_sprite(v::sprite s) {
    s.grid_size.y *= -1;
    m += s;
  }
};

hai::uptr<v::mapper> v::map() {
  return hai::uptr<v::mapper> { new ::mapper {} };
}

static void on_frame() {
  if (!vv::as()->ppl.program()) return;

  static bool loaded = false;
  if (!loaded) {
    using namespace gelo;
    auto p = vv::as()->ppl.program();
    v::g_u_aspect = get_uniform_location(p, "pc.aspect");
    loaded = true;
  }
  v::call_on_frame();
  v::render();
}

const int i = [] {
  vv::setup(on_frame);
  return 0;
}();
