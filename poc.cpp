#pragma leco app

import casein;
import dotz;
import jute;
import hai;
import hashley;
import map;
import roomdefs;
import sprdef;
import tiledefs;
import traits;
import v;

static hashley::niamh g_spr_map { 1 };

[[nodiscard]] constexpr auto uv(unsigned i) {
  return dotz::ivec2 { i % 64, i / 64 };
}
[[nodiscard]] auto uv(jute::view id, unsigned idx = 0) {
  return uv(g_spr_map[id] + idx);
}

static map g_map {};

static void load(auto m) {
  for (auto y = 0; y < g_map.h; y++) {
    for (auto x = 0; x < g_map.w; x++) {
      auto c = g_map.data[y][x];
      if (!c) continue;
      dotz::ivec2 cuv = (c & map::wall)
        ? uv("environment/walls/brick_clay", c & 0xffff)
        : uv(c);
      m->push({ .pos { x, y }, .uv = cuv });
    }
  }
}

static void on_frame() {
  g_map.build();

  load(v::map());
  v::on_frame = [] {};

  casein::handle(casein::KEY_DOWN, casein::K_SPACE, on_frame);
}

const int i = [] {
  sprdef::load([](auto map) {
    g_spr_map = traits::move(map);

    tiledefs::load(g_spr_map, [](auto tiles) {
    });

    roomdefs::load(g_spr_map, [](auto rooms) {
      g_map.roomdefs = traits::move(rooms);

      v::pc = { 16, 16 };
      v::on_frame = on_frame;
    });
  });
  return 0;
}();
