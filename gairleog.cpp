#pragma leco app

import audio;
import casein;
import console;
import dotz;
import enemies;
import entdefs;
import ents;
import errs;
import fx;
import glispy;
import hai;
import hitdefs;
import imgui;
import inv;
import jute;
import lights;
import lootfx;
import map;
import res;
import rng;
import save;
import sicfg;
import silog;
import sitime;
import splats;
import sprdef;
import sfxdefs;
import strings;
import sv;
import v;

#define BLINK "\e[5m"
#define VERSION "1.0.0-beta"

static void reset_keys() {
  using namespace casein;
  reset_k(KEY_DOWN);
  reset_k(KEY_UP);
}

static void on_game();
static void on_game_over();
static void on_main_menu();

static void draw_console(auto & m) {
  auto font = sprdef::get("font").id;

  m->set_grid({ 16, 16 });
  auto w = 16.f * casein::window_size.x / casein::window_size.y - 16.f;

  using namespace imgui;
  start(&*m, { -w + 0.5f, 31.5f - console::lines }, [&] {
    vbox([&] {
      console::for_each([&](auto secs, auto txt) {
        float a = 1.0f - dotz::clamp(secs * 0.2f, 0.0f, 1.0f);
        mult(a, [&] {
          hbox([&] { text(font, txt); });
        });
      });
    });
  });
}

// TODO: fix attack dice roll
// TODO: increase enemy stats after each level
// TODO: balance enemy health
// TODO: some visual feedback for near-death
// TODO: add sounds for more entities
// TODO: add more colour variations
static void on_game_frame() {
  static sitime::stopwatch timer {};
  float timer_a = dotz::sinf(timer.secs() * 3);

  lights::tick();

  dotz::vec2 player_pos {};
  bool dead = true;
  ents::foreach([&](const auto & e) {
    if (!e.flags.player) return;
    player_pos = e.pos;
    dead = false;
  });
  if (dead) {
    on_game_over();
    return;
  }

  auto m = v::map();
  m->set_grid({ player_pos + 0.5f, 6 });
  splats::for_each([&](const auto & d) {
    m->push({
      .pos = d.pos,
      .mult { 1, 1, 1, 0.3f },
      .id = d.sprite,
    });
  });
  ents::foreach([&](const auto & d) {
    auto p = d.pos;
    if (d.size.x < 1) p.x++;
    dotz::vec3 l = dotz::pow(lights::at(d.pos).current * 3.0, 1.5);
    auto c = d.poison
      ? dotz::vec3 { timer_a, 1, timer_a }
      : dotz::vec3 { 1 };
    // TODO: feedback anim from last_moved_timestamp
    m->push({
      .pos = p,
      .scale = d.size,
      .mult { l * c, 1.f },
      .id = d.sprite,
    });
  });
  fx::draw(m);
  draw_console(m);
}

// TODO: drop a random item on level change
static void on_start_level() try {
  save::current_stage++;
  glispy::reset();
  ents::reset();
  map::build();
  lights::reset();
  splats::reset();
  on_game();
} catch (const lispy::parser_error & e) {
  silog::die("%s", lispy::to_file_err(e).begin());
} catch (const hai::cstr & err) {
  silog::die("%s", err.begin());
}
static void on_exit() try {
  switch (lootfx::apply_by_name(strings::get("level-exit-fx"))) {
    using enum lootfx::outcome;
    case death: return on_game_over();
    case none: return on_start_level();
  }
} catch (const lispy::parser_error & e) {
  silog::die("%s", lispy::to_file_err(e).begin());
} catch (const hai::cstr & err) {
  silog::die("%s", err.begin());
}

static jute::heap g_game_over_msg = "";
static void do_game_over() {
  auto font = sprdef::get("font").id;

  auto m = v::map();
  m->set_grid({ {9.f,3.f}, 9 });

  using namespace imgui;
  start(&*m, {}, [&] {
    vbox([&] {
      scale({ 2.0f }, [&] {
        hbox([&] { text(font, "Game Over"); });
      });
      space();
      scale({ 0.5f }, [&] {
        hbox([&] {
          auto x = (9.f / 0.5f) - g_game_over_msg.size() / 2.0f;
          scale({ x }, [] { space(); });
          text(font, g_game_over_msg);
        });
      });
      space();
      hbox([&] {
        scale({ 2.5f }, [] { space(); });
        text(font, BLINK "Press any key");
      });
    });
  });
}
static void on_game_over() {
  reset_keys();
  g_game_over_msg = strings::get("gameover-sub");

  save::reset([] {
    using namespace casein;
    handle(KEY_DOWN, on_main_menu);

    v::on_frame = do_game_over;
  });
}

static sitime::stopwatch g_sel_anim {};
static int g_sel = 0;
static int g_tgt_sel = 0;

static void on_inv_use() try {
  switch (lootfx::apply(inv::at(g_sel).sprite)) {
    using enum lootfx::outcome;
    case death: return on_game_over();
    case none: break;
  }
  inv::consume(g_sel);
  reset_keys();
  g_sel_anim = {};
  if (g_sel == inv::size()) {
    g_tgt_sel = g_sel ? g_sel - 1 : 0;
  } else {
    g_tgt_sel = g_sel;
    g_sel -= 1;
  }
} catch (const lispy::parser_error & e) {
  silog::die("%s", lispy::to_file_err(e).begin());
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
    m->set_grid({ 0, 12 });

    ents::t player {};
    ents::foreach({ .player = true }, [&](auto p) {
      player = p;
    });
    if (!player.life) return on_game_over();

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
            space();
            if (player.poison) {
              hbox([&] {
                text(font, "Poison: ");
                number(font, player.poison);
              });
              scale(0.5f, space);
            }
            hbox([&] {
              text(font, "Str.:   ");
              number(font, player.strength);
            });
            scale(0.5f, space);
            hbox([&] {
              text(font, "Def.:   ");
              number(font, player.defense);
            });
            space();
            hbox([&] {
              text(font, "Stage:  ");
              number(font, save::current_stage);
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
        .mult { a, a, a, 1.f },
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

    draw_console(m);
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
    } catch (const lispy::parser_error & e) {
      silog::die("%s", lispy::to_file_err(e).begin());
    } catch (const hai::cstr & msg) {
      silog::die("Error: %s", msg.begin());
    }
  };
}

static void on_game() {
  reset_keys();

  v::on_frame = on_game_frame;

  using namespace casein;
  handle(KEY_DOWN, K_UP,    move(0, -1));
  handle(KEY_DOWN, K_DOWN,  move(0, +1));
  handle(KEY_DOWN, K_LEFT,  move(-1, 0));
  handle(KEY_DOWN, K_RIGHT, move(+1, 0));

  handle(KEY_DOWN, K_TAB,    on_inventory);
#ifndef LECO_TARGET_WASM
  handle(KEY_DOWN, K_ESCAPE, [] {
    save::store(on_main_menu);
  });
#endif
}

static void on_start() {
  rng::seed();
  lootfx::reset();
  inv::reset();
  console::reset();
  v::on_frame = on_start_level;
}
#ifndef LECO_TARGET_WASM
static void on_continue() {
  v::on_frame = on_game;
}
#endif

static void menu(int * opt, bool * clk) {
  reset_keys();

  using namespace casein;
  handle(KEY_DOWN, K_UP, [opt] {
    (*opt)--;
    sfxdefs::play("menu_selection");
  });
  handle(KEY_DOWN, K_DOWN, [opt] {
    (*opt)++;
    sfxdefs::play("menu_selection");
  });
  handle(KEY_DOWN, K_ENTER, [clk] {
    *clk = true;
    sfxdefs::play("menu_click");
  });
}

static void do_main_menu();

#ifndef LECO_TARGET_WASM
static int g_opt_sel = 0;
static bool g_opt_clk = false;
static void on_options() {
  menu(&g_opt_sel, &g_opt_clk);

  v::on_frame = [] {
    auto m = v::map();
    m->set_grid({ {8,4}, 8 });

    auto font = sprdef::get("font").id;
    auto mark = entdefs::get("player").sprite;

    int id = 0;

    using namespace imgui;

    const auto menu_item = [&](sv name, auto && extra) -> bool {
      bool clicked = false;
      hbox([&] {
        if (g_opt_clk && g_opt_sel == id) clicked = true;;
        sprite(g_opt_sel == id ? mark : 0);
        space();
        if (g_opt_sel == id) text(font, jute::fmt<BLINK "%s">(name));
        else text(font, name);
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
#endif

static void on_credits() {
  reset_keys();

  v::on_frame = [] {
    auto m = v::map();
    m->set_grid({ 12, 12 });

    auto font = sprdef::get("font").id;

    using namespace imgui;

    const auto center = [&](float sc, sv str) {
      scale(sc, [&] {
        hbox([&] {
          scale((24 / sc - str.size()) / 2.f, [] { space(); });
          text(font, str);
        });
        scale(0.5f, space);
      });
    };

    start(&*m, {}, [&] {
      vbox([&] {
        scale(6.6f, space);
        center(1.0f, "Game developed by");
        center(1.0f, "Eduardo 'm4c0' Costa");
        center(1.0f, "https://m4c0.itch.io");
        scale(1.5f, space);
        center(0.8f, "Using assets from:");
        center(0.8f, "PixeLike II Asset Pack");
        center(0.8f, "DUNGEON.mode");
#ifndef LECO_TARGET_WASM
        center(0.8f, "Minifantasy SFX Pack");
#endif
      });
    });
  };

  using namespace casein;
  handle(KEY_DOWN, K_ESCAPE, on_main_menu);
}

static int g_menu_sel = 0;
static bool g_menu_clk = false;
static void do_main_menu() {
  menu(&g_menu_sel, &g_menu_clk);

  v::on_frame = [] {
    auto m = v::map();
    m->set_grid({ {8,4}, 8 });

    auto font = sprdef::get("font").id;
    auto mark = entdefs::get("player").sprite;

    int id = 0;

    using namespace imgui;

    const auto menu_item = [&](bool enabled, sv name) -> bool {
      bool clicked = false;
      hbox([&] {
        if (enabled && g_menu_clk && g_menu_sel == id) clicked = true;;
        sprite(enabled && g_menu_sel == id ? mark : 0);
        space();
        mult(enabled ? 1.0 : 0.2, [&] {
          if (g_menu_sel == id) text(font, jute::fmt<BLINK "%s">(name));
          else text(font, name);
        });
        if (enabled) id++;
      });
      return clicked;
    };

    start(&*m, { 0, -1 }, [&] {
      vbox([&] {
        scale({ 2.0f }, [&] {
          hbox([&] {
            text(font, "Gairleog");
          });
        });
        hbox([&] {
          scale({ 3.f }, [&] {
            space();
          });
          scale({ 0.75f }, [&] {
            text(font, "Swamp of doom");
          });
        });
        hbox([&] {});

        using namespace casein;
        if (menu_item(true,           "New Game")) on_start();
#ifndef LECO_TARGET_WASM
        if (menu_item(save::exists(), "Continue")) on_continue();
        if (menu_item(true,           "Options"))  on_options();
#endif
        if (menu_item(true,           "Credits"))  on_credits();
#ifndef LECO_TARGET_WASM
        if (menu_item(true,           "Exit"))     interrupt(IRQ_QUIT);
#endif
      });
    });
    start(&*m, { 0, 11 }, [&] {
      hbox([&] {
        scale(0.5f, [&] {
          scale(6.5f, space);
          text(font, "Version: " VERSION);
        });
      });
    });

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

  do_main_menu();
}

const int i = [] {
  try {
    audio::init();
    audio::enabled = !sicfg::boolean("mute");
    casein::fullscreen = !sicfg::boolean("windowed");
    res::load_all(on_init);
    return 0;
  } catch (const lispy::parser_error & e) {
    silog::die("%s", lispy::to_file_err(e).begin());
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
  }
}();
