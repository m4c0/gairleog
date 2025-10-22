#pragma leco app

import casein;
import dotz;
import ents;
import hai;
import hitdefs;
import inv;
import map;
import res;
import silog;
import sitime;
import sprdef;
import tiledefs;
import v;

static map g_map {};

static void on_frame() {
  static sitime::stopwatch ms {};

  ents::foreach([&](const auto & e) {
    if (!e.flags.player) return;
    g_map.tick_lights(e.pos, e.light, ms.millis());
    v::pc = { e.pos + 0.5f, 6 };
  });
  ms = {};

  auto m = v::map();
  ents::foreach([&](const auto & d) {
    m->push({
      .pos = d.pos,
      .mult = g_map.at(d.pos).clight,
      .id = d.sprite,
    });
  });
}

static void on_game();

static void on_exit() try {
  ents::reset();
  g_map.build();
  g_map.foreach(ents::add);
  on_game();
} catch (const hai::cstr & err) {
  silog::die("%s", err.begin());
}

static void on_inventory() {
  using namespace casein;
  reset_k(KEY_DOWN);
  reset_k(KEY_UP);

  handle(KEY_DOWN, K_ESCAPE, on_game);

  v::on_frame = [] {
    auto m = v::map();
    auto y = 0;
    for (auto & i : inv::all()) {
      m->push({
        .pos { 0, y++ },
        .id = i.sprite,
      });
    }
    v::pc = { 0, 6 };
  };
}

static constexpr const auto move(int dx, int dy) {
  return [=] {
    ents::foreach([&](auto & p) {
      if (!p.flags.player) return;
      switch (ents::move(&p, { dx, dy })) {
        using enum ents::move_outcome;
        case none: break;
        case exit: v::on_frame = on_exit; break;
      }
    });
  };
}

static void on_game() {
  v::on_frame = on_frame;

  using namespace casein;
  handle(KEY_DOWN, K_UP,    move(0, -1));
  handle(KEY_DOWN, K_DOWN,  move(0, +1));
  handle(KEY_DOWN, K_LEFT,  move(-1, 0));
  handle(KEY_DOWN, K_RIGHT, move(+1, 0));

  handle(KEY_DOWN, K_TAB, on_inventory);
}

const int i = [] {
  try {
    res::load_all([] {
      v::on_frame = on_exit;
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
  }

  return 0;
}();
