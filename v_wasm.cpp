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

void v::mapper::add_sprite(v::sprite s) {
  s.grid_size.y *= -1;
  m += s;
}

static void on_frame() {
  v::vv::ss()->frame([] {
    if (!v::vv::as()->ppl.program()) return;
    using namespace gelo;

    static int u_aspect = 0;
    if (!u_aspect) {
      auto p = v::vv::as()->ppl.program();
      u_aspect = get_uniform_location(p, "pc.aspect");
    }
    v::call_on_frame();

    uniform1f(u_aspect, casein::window_size.x / casein::window_size.y);

    v::vv::ss()->clear({ 0, 0, 0, 1 });
    v::vv::as()->ppl.cmd_draw();
  });
}

const int i = [] {
  v::vv::setup(on_frame);
  return 0;
}();
