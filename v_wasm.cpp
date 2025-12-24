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
struct ext_stuff : v::base_extent_stuff {
  ext_stuff() : v::base_extent_stuff { vv::as() } {}
};

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
  vv::ss()->frame([] {
    if (!vv::as()->ppl.program()) return;
    using namespace gelo;

    static bool loaded = false;
    static int u_aspect;
    if (!loaded) {
      auto p = vv::as()->ppl.program();
      u_aspect = get_uniform_location(p, "pc.aspect");
      loaded = true;
    }
    v::call_on_frame();

    uniform1f(u_aspect, casein::window_size.x / casein::window_size.y);

    vv::ss()->clear({ 0, 0, 0, 1 });
    vv::as()->ppl.cmd_draw();
  });
}

const int i = [] {
  vv::setup(on_frame);
  return 0;
}();
