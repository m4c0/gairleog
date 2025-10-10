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

static tiledefs::t player_tdef;

static auto player_pos() {
  dotz::ivec2 p { 1 };
  ents::foreach([&](const auto & e) {
    if (!e.flags.player) return;
    p = e.pos;
  });
  return p;
}

static void on_frame() {
  auto ppos = player_pos();

  static sitime::stopwatch ms {};
  g_map.tick_lights(ppos, player_tdef.light, ms.millis());
  ms = {};

  v::pc = { ppos + 0.5f, 6 };

  auto m = v::map();
  ents::foreach([&](const auto & d) {
    m->push({
      .pos = d.pos,
      .mult = g_map.at(d.pos).clight,
      .id = d.sprite,
    });
  });
}

static void on_exit() {
  ents::reset();
  g_map.build();
  g_map.foreach(ents::add);
  v::on_frame = on_frame;
}

static bool player_hitcheck(ents::t & tgt) {
  bool result = true;
  for (auto act : hitdefs::check(player_tdef.flags, tgt.flags)) {
    ents::take_hit(&tgt, act);
    switch (act) {
      using enum hitdefs::action;
      case block:
      case hit:
      case miss:
      case poison: {
        // We hit something. Stop the player
        result = false;
        break;
      }
      case pick: break;
      case exit: {
        v::on_frame = on_exit;
        return false;
      }
    }
  }
  return result;
}

static constexpr const auto move(int dx, int dy) {
  return [=] {
    ents::foreach([&](auto & p) {
      if (!p.flags.player) return;

      auto p_pos = p.pos + dotz::ivec2 { dx, dy };
      ents::foreach([&](auto & d) {
        if (d.pos != p_pos) return;
        if (!player_hitcheck(d)) p_pos = p.pos;
      });
      p.pos = p_pos;
    });
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
