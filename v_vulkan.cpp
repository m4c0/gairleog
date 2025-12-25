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

struct mapper : v::mapper {
  decltype(v::vv::as()->ppl.map()) m = v::vv::as()->ppl.map();

  void add_sprite(v::sprite s) override { m += s; }
};
hai::uptr<v::mapper> v::map() {
  return hai::uptr<v::mapper> { new ::mapper {} };
}

// TODO: fix a rare sync issue between player and rest of game
static void on_frame() try {
  v::vv::ss()->frame([] {
    v::call_on_frame();

    auto rp = v::vv::ss()->clear({ 0, 0, 0, 1 });

    auto cb = v::vv::ss()->sw.command_buffer();
    float pc = v::vv::ss()->sw.aspect();
    v::vv::as()->ppl.cmd_draw(cb, &pc);
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
