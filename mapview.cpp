#pragma leco app

import casein;
import hai;
import map;
import res;
import silog;
import sires;
import v;

static map g_map {};

static void load() {
  auto m = v::map();
  g_map.foreach([&](auto pos, const auto & d) {
    m->push({
      .pos = pos,
      .id = d.sprite,
    });
  });
}
static void on_frame() {
  g_map.build();
  load();
  v::on_frame = [] {};

  casein::handle(casein::KEY_DOWN, casein::K_SPACE, on_frame);
}

const int i = [] {
  try {
    res::load_all([] {
      v::pc = { 16, 16 };
      v::on_frame = on_frame;
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
    throw;
  }
  return 0;
}();
