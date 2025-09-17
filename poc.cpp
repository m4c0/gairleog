#pragma leco app

import casein;
import map;
import res;
import silog;
import sires;
import v;

static map g_map {};

static void on_frame() {
  g_map.build();
  g_map.load(v::map());
  v::on_frame = [] {};

  casein::handle(casein::KEY_DOWN, casein::K_SPACE, on_frame);
}

const int i = [] {
  res::on_error([](auto msg) { silog::die("Failure loading resource: %s", msg.cstr().begin()); });
  res::load_all([] {
    v::pc = { 16, 16 };
    v::on_frame = on_frame;
  });
  return 0;
}();
