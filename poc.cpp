#pragma leco app

import dotz;
import jute;
import hai;
import hashley;
import map;
import roomdefs;
import sprdef;
import traits;
import v;
import wallbuilder;

static hashley::niamh g_spr_map { 1 };
static hai::varray<hai::sptr<roomdefs::t>> g_roomdefs {};

[[nodiscard]] constexpr auto uv(unsigned i) {
  return dotz::ivec2 { i % 64, i / 64 };
}
[[nodiscard]] auto uv(jute::view id, unsigned idx = 0) {
  return uv(g_spr_map[id] + idx);
}

static map g_map {};

template<unsigned W, unsigned H>
static void load(auto & m, unsigned (&map)[H][W]) {
  for (auto y = 0; y < H; y++) {
    for (auto x = 0; x < W; x++) {
      auto c = map[y][x];
      if (!c) continue;
      m->push({ .pos { x, y }, .uv = uv("environment/walls/brick_clay", c) });
    }
  }
}

static void on_frame() {
  g_map.build();
  wallbuilder::draw(g_map.data);

  auto m = v::map();
  load(m, g_map.data);
  v::on_frame = [] {};
}

static void start() {
  v::pc = { 16, 16 };
  v::on_frame = on_frame;
}

const int i = [] {
  sprdef::load([](auto map) {
    g_spr_map = traits::move(map);

    roomdefs::load(g_spr_map, [&](auto rooms) {
      g_roomdefs = traits::move(rooms);

      start();
    });
  });
  return 0;
}();
