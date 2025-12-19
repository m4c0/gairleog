#pragma leco app

import casein;
import enemies;
import ents;
import errs;
import glispy;
import hai;
import mainloop;
import map;
import res;
import save;
import silog;
import sires;
import v;

static void load() {
  auto m = v::map();
  m->set_grid({ 16, 16 });
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
static void on_refresh() {
  ents::reset();
  map::build();

  load();

  using namespace casein;
  v::on(KEY_DOWN, K_TAB,   tick);
  v::on(KEY_DOWN, K_SPACE, on_refresh);

  v::on(KEY_DOWN, K_UP,    move(0, -1));
  v::on(KEY_DOWN, K_DOWN,  move(0, +1));
  v::on(KEY_DOWN, K_LEFT,  move(-1, 0));
  v::on(KEY_DOWN, K_RIGHT, move(+1, 0));

  v::on(KEY_DOWN, K_MINUS, [] {
    if (--save::current_stage < 1) save::current_stage = 1;
    glispy::reset();
    on_refresh();
  });
  v::on(KEY_DOWN, K_EQUAL, [] {
    ++save::current_stage;
    glispy::reset();
    on_refresh();
  });
}

static void on_static_init() {
  save::current_stage = 1;
  glispy::reset();
  res::load_all(on_refresh);
}
const int i = [] {
  v::push(on_static_init);
  return 0;
}();
