#pragma leco app

import audio;
import casein;
import dotz;
import enemies;
import entdefs;
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

static void on_inv_use() try {
  ents::t * player;
  ents::foreach({ .player = true }, [&](auto & p) {
    player = &p;
  });
  if (!player || player->life == 0) return;

  for (auto act : lootfx::apply(inv::at(g_sel).sprite)) {
    switch (act) {
      using enum lootfx::action;
      case damage:
        // TODO: game over
        if (!ents::take_hit(player)) return on_game();
        break;
      case defence: player->defense++; break;
      case heal: if (player->life < player->max_life) player->life++; break;
      case max_life: player->max_life++; break;
      case poison: player->poison++; break;
      case strength: player->strength++; break;
      case weakness: if (player->strength > 0) player->strength--; break;
      case wither: if (player->defense > 0) player->defense--; break;
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
} catch (const hai::cstr & msg) {
  silog::die("Error: %s", msg.begin());
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

  handle(KEY_DOWN, K_ENTER, on_inv_use);
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
    // TODO: game over
    if (!player.life) return on_game();

    using namespace imgui;
    start(&*m, { -14.f, -1.5f }, [&] {
      hbox([&] {
        scale({ 0.75f }, [&] {
          vbox([&] {
            hbox([&] {
              text(font, "Life:   ");
              number(font, player.life);
              text(font, "/");
              number(font, player.max_life);
            });
            if (player.poison) {
              space({ 0.1f });
              hbox([&] {
                text(font, "Poison: ");
                number(font, player.poison);
              });
            }
            space({ 0.1f });
            hbox([&] {
              text(font, "Str.:   ");
              number(font, player.strength);
            });
            space({ 0.1f });
            hbox([&] {
              text(font, "Def.:   ");
              number(font, player.defense);
            });
          });
        });
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
      print({ .pos { -2.0f, -0.5f } }, "No items");
    }

    auto ms = g_sel_anim.millis() * 10;
    if (ms > 1000) g_sel = g_tgt_sel;

    auto dt = dotz::clamp(ms / 1000.0f, 0.0f, 1.0f);
    auto dd = (g_tgt_sel - g_sel) * dt;

    for (auto y = -5; y <= 5; y++)  {
      auto & i = inv::at(y + g_sel);
      if (!i.sprite) continue;

      float ry = y - dd;

      auto a = 1.0f - dotz::pow(dotz::abs(ry / 6.0f), 0.5f);
      auto s = 1.0f / (dotz::abs(ry / 5.0f) + 1.0f);

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
        hbox([&] {
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
      });
    }
    v::set_grid({ 0, 12 });
  };

  g_sel = 0;
}

static constexpr const auto move(int dx, int dy) {
  return [=] {
    try {
      ents::foreach({ .player = true }, [&](auto & p) {
        switch (ents::move(&p, { dx, dy })) {
          using enum ents::move_outcome;
          case none: {
            enemies::tick();
            break;
          }
          case exit: v::on_frame = on_exit; break;
        }
      });
    } catch (const hai::cstr & msg) {
      silog::die("Error: %s", msg.begin());
    }
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

static void on_start() {
  lootfx::reset();
  inv::reset();
  v::on_frame = on_exit;
}

static int g_menu_sel = 0;
static void on_main_menu() {
  using namespace casein;
  reset_k(KEY_DOWN);
  reset_k(KEY_UP);

  handle(KEY_DOWN, K_UP,   [] { g_menu_sel = (g_menu_sel + 3) % 2; });
  handle(KEY_DOWN, K_DOWN, [] { g_menu_sel = (g_menu_sel + 1) % 2; });

  handle(KEY_DOWN, K_ENTER, [] {
    switch (g_menu_sel) {
      case 0: on_start(); break;
      case 1: interrupt(IRQ_QUIT); break;
    }
  });

  v::on_frame = [] {
    auto m = v::map();

    auto font = sprdef::get("font").id;
    auto mark = entdefs::get("player").sprite;

    using namespace imgui;
    start(&*m, {}, [&] {
      vbox([&] {
        hbox([&] {
          sprite(g_menu_sel == 0 ? mark : 0);
          space({ 0.5f });
          text(font, "New Game");
        });
        hbox([&] {
          sprite(0);
          space({ 0.5f });
          mult(0.2, [&] {
            text(font, "Continue");
          });
        });
        hbox([&] {
          sprite(g_menu_sel == 1 ? mark : 0);
          space({ 0.5f });
          text(font, "Exit");
        });
      });
    });
    v::set_grid({ 6, 12 });
  };
}

const int i = [] {
  try {
    res::load_all([] {
      on_main_menu();
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
  }

  return 0;
}();
