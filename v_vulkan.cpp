module v;
import :tools;
import casein;
import dotz;
import hai;
import lispy;
import silog;
import sires;
import traits;
import vee;
import vinyl;
import voo;
import wagen;

void v::mapper::add_sprite(v::sprite s) { m += s; }

// TODO: fix a rare sync issue between player and rest of game
static void on_frame() {
  v::vv::ss()->frame([] {
    v::call_on_frame();

    auto rp = v::vv::ss()->clear({ 0, 0, 0, 1 });

    auto cb = v::vv::ss()->sw.command_buffer();
    float pc = v::vv::ss()->sw.aspect();
    v::vv::as()->ppl.cmd_draw(cb, &pc);
  });
}

const int i = [] {
  v::vv::setup(on_frame);
  return 0;
}();
