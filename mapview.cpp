#pragma leco app

import casein;
import enemies;
import ents;
import errs;
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
static void tick() {
  enemies::tick();
  load();
}
static constexpr const auto move(int dx, int dy) {
  return [=] {
    ents::foreach({ .player = true }, [&](auto & p) {
      ents::move(&p, { dx, dy });
      load();
    });
  };
}
static void on_frame() try {
  ents::reset();
  map::build();

  load();
  v::set_grid({ 16, 16 });
  v::on_frame = [] {};

  using namespace casein;

  handle(KEY_DOWN, K_TAB,   tick);
  handle(KEY_DOWN, K_SPACE, on_frame);

  handle(KEY_DOWN, K_UP,    move(0, -1));
  handle(KEY_DOWN, K_DOWN,  move(0, +1));
  handle(KEY_DOWN, K_LEFT,  move(-1, 0));
  handle(KEY_DOWN, K_RIGHT, move(+1, 0));
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
