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

struct mapper : v::mapper {
  decltype(v::vv::as()->ppl.map()) m = v::vv::as()->ppl.map();

  void add_sprite(v::sprite s) {
    s.grid_size.y *= -1;
    m += s;
  }
};

hai::uptr<v::mapper> v::map() {
  return hai::uptr<v::mapper> { new ::mapper {} };
}

static void on_frame() try {
  v::vv::ss()->frame([] {
    if (!v::vv::as()->ppl.program()) return;
    using namespace gelo;

    static bool loaded = false;
    static int u_aspect;
    if (!loaded) {
      auto p = v::vv::as()->ppl.program();
      u_aspect = get_uniform_location(p, "pc.aspect");
      loaded = true;
    }
    v::call_on_frame();

    uniform1f(u_aspect, casein::window_size.x / casein::window_size.y);

    v::vv::ss()->clear({ 0, 0, 0, 1 });
    v::vv::as()->ppl.cmd_draw();
  });
} catch (const lispy::parser_error & e) {
  silog::die("%s", lispy::to_file_err(e).begin());
} catch (const hai::cstr & msg) {
  silog::die("Error: %s", msg.begin());
}

const int i = [] {
  v::vv::setup(on_frame);
  return 0;
}();
