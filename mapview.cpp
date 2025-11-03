#pragma leco app

import casein;
import ents;
import hai;
import map;
import res;
import silog;
import sires;
import v;

static void load() {
  auto m = v::map();
  ents::foreach([&](const auto & d) {
    m->push({
      .pos = d.pos,
      .id = d.sprite,
    });
  });
}
static void on_frame() try {
  ents::reset();

  map m {};
  m.build();
  m.foreach(ents::add);

  load();
  v::set_grid({ 16, 16 });
  v::on_frame = [] {};

  casein::handle(casein::KEY_DOWN, casein::K_SPACE, on_frame);
} catch (const hai::cstr & err) {
  silog::die("%s", err.begin());
}

const int i = [] {
  try {
    res::load_all([] {
      v::on_frame = on_frame;
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
    throw;
  }
  return 0;
}();
