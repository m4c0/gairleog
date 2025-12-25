module v;
import :tools;
import casein;
import gelo;

void v::mapper::add_sprite(v::sprite s) {
  s.grid_size.y *= -1;
  m += s;
}

void v::extent_stuff::render() {
  using namespace gelo;

  static int u_aspect = 0;
  if (!u_aspect) {
    auto p = v::vv::as()->ppl.program();
    u_aspect = get_uniform_location(p, "pc.aspect");
  }

  uniform1f(u_aspect, casein::window_size.x / casein::window_size.y);

  v::vv::as()->ppl.cmd_draw();
}
