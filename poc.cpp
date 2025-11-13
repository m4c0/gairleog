#pragma leco app

import audio;
import casein;
import dotz;
import enemies;
import ents;
import fx;
import hai;
import hitdefs;
import imgui;
import inv;
import jute;
import lootfx;
import map;
import res;
import silog;
import sitime;
import sprdef;
import sv;
import tiledefs;
import v;

static map g_map {};

static void reset_keys() {
  using namespace casein;
  reset_k(KEY_DOWN);
  reset_k(KEY_UP);
}

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
    auto p = d.pos;
    if (d.size.x < 1) p.x++;
    m->push({
      .pos = p,
      .scale = d.size,
      .mult = g_map.at(d.pos).clight,
      .id = d.sprite,
    });
  });
  fx::draw(m);
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

static sitime::stopwatch g_sel_anim {};
static int g_sel = 0;
static int g_tgt_sel = 0;
static float inv_alpha(int y) {
  switch (dotz::abs(y)) {
    case 0: return 1.0;
    case 1: return 0.2;
    case 2: return 0.08;
    case 3: return 0.02;
    default: return 0.0;
  }
}
static void inv_setup() {
  using namespace casein;

  handle(KEY_DOWN, K_ESCAPE, on_game);
  handle(KEY_DOWN, K_TAB,    on_game);
  handle(KEY_DOWN, K_UP,   [] {
    reset_keys();
    g_sel_anim = {};
    g_tgt_sel = (g_sel == 0) ? 0 : g_sel - 1;
  });
  handle(KEY_DOWN, K_DOWN, [] {
    reset_keys();
    g_sel_anim = {};
    g_tgt_sel = (g_sel < inv::size() - 1) ? g_sel + 1 : g_sel;
  });

  handle(KEY_DOWN, K_ENTER, [] {
    for (auto act : lootfx::apply(inv::at(g_sel).sprite)) {
      switch (act) {
        using enum lootfx::action;
        case heal: silog::log(silog::debug, "heal"); break;
        case str:  silog::log(silog::debug, "str"); break;
      }
    }
    inv::consume(g_sel);
    if (g_sel == inv::size()) {
      reset_keys();
      g_sel_anim = {};
      g_tgt_sel = g_sel - 1;
    } else {
      reset_keys();
      g_sel_anim = {};
      g_tgt_sel = g_sel;
      g_sel -= 1;
    }
  });
}
static void on_inventory() {
  reset_keys();

  v::on_frame = [] {
    if (g_sel == g_tgt_sel) inv_setup();

    auto font = sprdef::get("font").id;
    auto m = v::map();

    ents::t player {};
    ents::foreach({ .player = true }, [&](auto p) {
      player = p;
    });
    if (!player.life) return on_game();

    using namespace imgui;
    start(&*m, { -8.f, -0.5f }, [&] {
      hbox([&] {
        scale({ 0.5f }, [&] {
          vbox([&] {
            hbox([&] {
              text(font, "Life: ");
              text(font, player.life);
            });
            space({ 0.1f });
            hbox([&] {
              text(font, "Str:  ");
              text(font, 1);
            });
            space({ 0.1f });
            hbox([&] {
              text(font, "Def:  ");
              text(font, 1);
            });
          });
        });
        // text(font, ">");
      });
    });

    const auto print = [&](v::sprite s, sv str) {
      for (int i = 0; i < str.size(); i++) {
        s.id = font + str[i];
        m->push(s);
        s.pos.x += s.scale.x;
      }
    };

    if (inv::size() > 0) {
      print({ .pos { -2.0f, -0.5f } }, ">");
    } else {
      print({ .pos { -0.0f, -0.5f } }, "No items");
    }

    auto ms = g_sel_anim.millis() * 10;
    if (ms > 1000) g_sel = g_tgt_sel;

    auto dt = dotz::clamp(ms / 1000.0f, 0.0f, 1.0f);
    auto dd = (g_tgt_sel - g_sel) * dt;

    for (auto y = -3; y <= 3; y++)  {
      auto & i = inv::at(y + g_sel);
      if (!i.sprite) continue;

      float ry = y - dd;

      auto a = inv_alpha(ry);
      auto s = 1.0f / (dotz::abs(ry / 3.0f) + 1.0f);

      float xx = -0.0f;
      float yy = (ry * 1.5f - 0.5f) * s;

      m->push({
        .pos { xx - s, yy },
        .scale { s },
        .mult = a,
        .id = i.sprite,
      });

      using namespace imgui;
      start(&*m, { xx, yy }, [&] {
        scale({ s }, [&] {
          mult(a, [&] {
            if (lootfx::has(i.sprite)) {
              text(font, lootfx::get(i.sprite));
            } else {
              text(font, "???");
            }
          });
        });
      });
    }
    v::set_grid({ 0, 6 });
  };

  g_sel = 0;
}

static constexpr const auto move(int dx, int dy) {
  return [=] {
    ents::foreach({ .player = true }, [&](auto & p) {
      switch (ents::move(&p, { dx, dy })) {
        using enum ents::move_outcome;
        case none: enemies::tick(); break;
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
      lootfx::reset();
      inv::reset();
      v::on_frame = on_exit;
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
  }

  return 0;
}();
