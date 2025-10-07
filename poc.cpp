#pragma leco app

import casein;
import dotz;
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
  g_map.load(m);
  m->push({ .pos = g_pos, .id = player_tdef.sprite });
}

static constexpr const auto move(int dx, int dy) {
  return [=] {
    auto p = g_pos + dotz::ivec2 { dx, dy };
    auto tgt = g_map.at(p).def.flags;
    for (auto act : hitdefs::check(player_tdef.flags, tgt)) {
      switch (act) {
        using enum hitdefs::action;
        case block: p = g_pos; break;
        case hit: silog::die("TODO: no hit yet");
        case miss: silog::die("TODO: no miss yet");
        case pick: silog::die("TODO: no pickyet");
        case poison: silog::die("TODO: no poison yet");
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
      v::on_frame = on_frame;
      g_map.build();
      g_map.at(g_pos) = {};
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
