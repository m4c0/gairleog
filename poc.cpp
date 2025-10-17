#pragma leco app

import casein;
import dotz;
import ents;
import hai;
import hitdefs;
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

static void on_exit() try {
  ents::reset();
  g_map.build();
  g_map.foreach(ents::add);
  v::on_frame = on_frame;
} catch (const hai::cstr & err) {
  silog::die("%s", err.begin());
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

const int i = [] {
  try {
    res::load_all([] {
      v::on_frame = on_exit;
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
  }

  using namespace casein;
  handle(KEY_DOWN, K_UP,    move(0, -1));
  handle(KEY_DOWN, K_DOWN,  move(0, +1));
  handle(KEY_DOWN, K_LEFT,  move(-1, 0));
  handle(KEY_DOWN, K_RIGHT, move(+1, 0));

  return 0;
}();
