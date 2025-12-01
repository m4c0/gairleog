#pragma leco app

import audio;
import casein;
import dotz;
import enemies;
import entdefs;
import ents;
import errs;
import fx;
import hai;
import hitdefs;
import imgui;
import inv;
import jute;
import lights;
import lootfx;
import map;
import save;
import sicfg;
import silog;
import sitime;
import sprdef;
import sv;
import v;
import wav;

static map g_map {};

static void reset_keys() {
  using namespace casein;
  reset_k(KEY_DOWN);
  reset_k(KEY_UP);
}

static void on_frame() {
  lights::tick();

  ents::foreach([&](const auto & e) {
    if (!e.flags.player) return;
    v::set_grid({ e.pos + 0.5f, 6 });
  });

  auto m = v::map();
  ents::foreach([&](const auto & d) {
    auto p = d.pos;
    if (d.size.x < 1) p.x++;
    auto l = dotz::pow(lights::at(d.pos).current * 3.0, 1.5);
    m->push({
      .pos = p,
      .scale = d.size,
      .mult = l,
      .id = d.sprite,
    });
  });
  fx::draw(m);
}

static void on_game();
static void on_main_menu();

// TODO: drop a random item on level change
static void on_exit() try {
  ents::reset();
  g_map.build();
  g_map.foreach(ents::add);
  lights::reset();
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

  handle(KEY_DOWN, K_TAB,    on_inventory);
  handle(KEY_DOWN, K_ESCAPE, on_main_menu);
}

static void on_start() {
  lootfx::reset();
  inv::reset();
  v::on_frame = on_exit;
}
static void on_continue() {
  v::on_frame = on_game;
}

static auto menu_click_wav = [] {
  try {
    return wav::load("01_human_atk_sword_1.wav");
  } catch (const wav::error & err) {
    silog::error("Could not load menu sound file");
    return hai::array<float> {};
  }
}();
static auto menu_change_wav = [] {
  try {
    return wav::load("20_human_walk_stone_1.wav");
  } catch (const wav::error & err) {
    silog::error("Could not load menu sound file");
    return hai::array<float> {};
  }
}();
static void menu(int * opt, bool * clk) {
  using namespace casein;
  reset_k(KEY_DOWN);
  reset_k(KEY_UP);

  handle(KEY_DOWN, K_UP, [opt] {
    (*opt)--;
    audio::play(menu_change_wav);
  });
  handle(KEY_DOWN, K_DOWN, [opt] {
    (*opt)++;
    audio::play(menu_change_wav);
  });
  handle(KEY_DOWN, K_ENTER, [clk] {
    *clk = true;
    audio::play(menu_click_wav);
  });
}

static void do_main_menu();

static int g_opt_sel = 0;
static bool g_opt_clk = false;
static void on_options() {
  menu(&g_opt_sel, &g_opt_clk);

  v::on_frame = [] {
    auto m = v::map();

    auto font = sprdef::get("font").id;
    auto mark = entdefs::get("player").sprite;

    int id = 0;

    using namespace imgui;

    const auto menu_item = [&](sv name, auto && extra) -> bool {
      bool clicked = false;
      hbox([&] {
        if (g_opt_clk && g_opt_sel == id) clicked = true;;
        sprite(g_opt_sel == id ? mark : 0);
        space({ 0.5f });
        text(font, name);
        extra();
        id++;
      });
      return clicked;
    };
    const auto opt_item = [&](sv name, bool v) -> bool {
      return menu_item(name, [&] {
        text(font, v ? "on"_sv : "off"_sv);
      });
    };

    start(&*m, {}, [&] {
      vbox([&] {
        scale({ 4.0f }, [&] {
          hbox([&] {});
        });
        if (opt_item("Fullscreen ", casein::fullscreen)) {
          casein::fullscreen = !casein::fullscreen;
          casein::interrupt(casein::IRQ_FULLSCREEN);
          sicfg::boolean("windowed", !casein::fullscreen);
        }
        if (opt_item("Sounds     ", audio::enabled)) {
          audio::enabled = !audio::enabled;
          audio::interrupt();
          sicfg::boolean("mute", !audio::enabled);
        }
        if (menu_item("Back", [] {})) v::on_frame = do_main_menu;
      });
    });

    if (g_opt_sel < 0) g_opt_sel = id - 1;
    if (g_opt_sel >= id) g_opt_sel = 0;
    if (g_opt_clk) g_opt_clk = false;
  };
}

static int g_menu_sel = 0;
static bool g_menu_clk = false;
static void do_main_menu() {
  menu(&g_menu_sel, &g_menu_clk);

  v::on_frame = [] {
    auto m = v::map();

    auto font = sprdef::get("font").id;
    auto mark = entdefs::get("player").sprite;

    int id = 0;
    bool has_cont = save::exists();

    using namespace imgui;

    const auto menu_item = [&](bool enabled, sv name) -> bool {
      bool clicked = false;
      hbox([&] {
        if (enabled && g_menu_clk && g_menu_sel == id) clicked = true;;
        sprite(enabled && g_menu_sel == id ? mark : 0);
        space({ 0.5f });
        mult(enabled ? 1.0 : 0.2, [&] {
          text(font, name);
        });
        if (enabled) id++;
      });
      return clicked;
    };

    start(&*m, {}, [&] {
      vbox([&] {
        scale({ 2.0f }, [&] {
          hbox([&] {
            text(font, "Gairleog");
          });
          hbox([] {});
        });
        if (menu_item(true,     "New Game")) on_start();
        if (menu_item(has_cont, "Continue")) on_continue();
        if (menu_item(true,     "Options"))  on_options();
#ifndef LECO_TARGET_WASM
        using namespace casein;
        if (menu_item(true,     "Exit"))     interrupt(IRQ_QUIT);
#endif
      });
    });
    v::set_grid({ {8,4}, 8 });

    if (g_menu_sel < 0) g_menu_sel = id - 1;
    if (g_menu_sel >= id) g_menu_sel = 0;
    if (g_menu_clk) g_menu_clk = false;
  };
}
static void on_init() {
  using namespace casein;
  reset_k(KEY_DOWN);
  reset_k(KEY_UP);

  v::on_frame = [] {};

  save::prefetch([] {
    g_menu_sel = save::exists() ? 1 : 0;
    do_main_menu();
  });
}
static void on_main_menu() {
  using namespace casein;
  reset_k(KEY_DOWN);
  reset_k(KEY_UP);

  v::on_frame = [] {};

  if (g_menu_sel == 0) g_menu_sel = 1;

  save::store(do_main_menu);
}

const int i = [] {
  audio::enabled = !sicfg::boolean("mute");
  casein::fullscreen = !sicfg::boolean("windowed");
  save::init(on_init);
  return 0;
}();
