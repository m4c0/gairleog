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

static dotz::ivec2 g_pos { 1 };

static tiledefs::t player_tdef;

static void on_frame() {
  static sitime::stopwatch ms {};
  g_map.tick_lights(g_pos, player_tdef.light, ms.millis());
  ms = {};

  v::pc = { g_pos + 0.5f, 6 };

  auto m = v::map();
  g_map.foreach([&](auto pos, const auto & d) {
    m->push({
      .pos = pos,
      .mult = d.clight,
      .id = d.sprite,
    });
  });
  ents::foreach([&](const auto & d) {
    m->push({
      .pos = d.pos,
      .mult = g_map.at(d.pos).clight,
      .id = d.sprite,
    });
  });
  m->push({ .pos = g_pos, .id = player_tdef.sprite });
}

static void on_exit() {
  ents::reset();
  g_pos = { 1 };

  g_map.build();
  g_map.foreach(ents::lift);
  g_map.at(g_pos) = {};
  v::on_frame = on_frame;
}

static constexpr const auto move(int dx, int dy) {
  return [=] {
    auto p = g_pos + dotz::ivec2 { dx, dy };
    auto tgt = g_map.at(p).flags;
    for (auto act : hitdefs::check(player_tdef.flags, tgt)) {
      switch (act) {
        using enum hitdefs::action;
        case block:
        case hit:
        case miss:
        case poison: {
          // We hit something. Stop the player
          p = g_pos;
          break;
        }
        case pick: break;
        case exit: {
          v::on_frame = on_exit;
          return;
        }
      }
    }

    g_pos = p;
  };
}

const int i = [] {
  try {
    res::load_all([] {
      player_tdef = {
        .light = 1,
        .sprite = sprdef::get("characters/human_knight"),
        .flags = {
          .player = true,
          .solid = true,
        },
      };
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
