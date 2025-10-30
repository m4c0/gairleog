#pragma leco app

import casein;
import dotz;
import ents;
import hai;
import hitdefs;
import inv;
import lootfx;
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
    v::set_grid({ e.pos + 0.5f, 6 });
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

// TODO: drop a random item on level change
static void on_exit() try {
  ents::reset();
  g_map.build();
  g_map.foreach(ents::add);
  on_game();
} catch (const hai::cstr & err) {
  silog::die("%s", err.begin());
}

static float inv_alpha(int y) {
  switch (dotz::abs(y)) {
    case 0: return 1.0;
    case 1: return 0.2;
    case 2: return 0.08;
    case 3: return 0.02;
    default: return 0.0;
  }
}
static void on_inventory() {
  static int sel;

  using namespace casein;
  reset_k(KEY_DOWN);
  reset_k(KEY_UP);

  handle(KEY_DOWN, K_ESCAPE, on_game);
  handle(KEY_DOWN, K_UP,   [] { sel = (sel == 0) ? 0 : sel - 1; });
  handle(KEY_DOWN, K_DOWN, [] { sel = (sel < inv::size() - 1) ? sel + 1 : sel; });

  handle(KEY_DOWN, K_ENTER, [] {
    lootfx::for_sprite(inv::at(sel).sprite).valid = true;
    inv::consume(sel);
    while (sel == inv::size()) sel--;
  });

  sel = 0;
  v::on_frame = [] {
    auto font = sprdef::get("font");
    auto m = v::map();
    if (inv::size() > 0) {
      m->push({
        .pos { -4.0f, -0.5f },
        .id = font + '>',
      });
    }

    for (auto y = -3; y <= 3; y++)  {
      auto & i = inv::at(y + sel);
      if (!i.sprite) continue;

      auto a = inv_alpha(y);

      m->push({
        .pos { -3.0f, y - 0.5f },
        .mult = a,
        .id = i.sprite,
      });
      if (lootfx::for_sprite(i.sprite).valid) {
        continue;
      }
      m->push({
        .pos { -2.0f, y - 0.5f },
        .mult = a,
        .id = font + '?',
      });
      m->push({
        .pos { -1.0f, y - 0.5f },
        .mult = a,
        .id = font + '?',
      });
      m->push({
        .pos { 0.0f, y - 0.5f },
        .mult = a,
        .id = font + '?',
      });
    }
    v::set_grid({ 0, 6 });
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
  reset_k(KEY_DOWN);
  reset_k(KEY_UP);

  handle(KEY_DOWN, K_UP,    move(0, -1));
  handle(KEY_DOWN, K_DOWN,  move(0, +1));
  handle(KEY_DOWN, K_LEFT,  move(-1, 0));
  handle(KEY_DOWN, K_RIGHT, move(+1, 0));

  handle(KEY_DOWN, K_TAB, on_inventory);
}

const int i = [] {
  try {
    res::load_all([] {
      inv::reset();
      v::on_frame = on_exit;
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
  }

  return 0;
}();
