#pragma leco app

import dotz;
import jute;
import sprdef;
import v;

[[nodiscard]] constexpr auto uv(unsigned i) {
  return dotz::ivec2 { i % 64, i / 64 };
}
[[nodiscard]] auto uv(jute::view id, unsigned idx = 0) {
  static const auto sprdefs = sprdef::load();
  return uv(sprdefs[id] + idx);
}

static int g_map[8][8] {
  { 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 1, 0, 0, 1 },
  { 1, 0, 1, 0, 1, 1, 0, 1 },
  { 1, 0, 1, 0, 1, 0, 0, 1 },
  { 1, 1, 1, 0, 0, 0, 0, 1 },
  { 1, 0, 1, 1, 0, 1, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1 },
};

static void on_frame() {
  for (auto y = 0; y < 8; y++) {
    for (auto x = 0; x < 8; x++) {
      if (g_map[y][x] == 0) continue;

      bool l = x > 0 && g_map[y][x - 1];
      bool r = x < 7 && g_map[y][x + 1];
      bool u = y > 0 && g_map[y - 1][x];
      bool d = y < 7 && g_map[y + 1][x];
      g_map[y][x] =
        l && r && u && d ? 19 :
        l && r && u ? 41 :
        l && r && d ? 19 :
        l && u && d ? 18 :
        r && u && d ? 15 :
        l && r ? 34 :
        u && l ? 24 :
        u && r ? 22 :
        u && d ? 14 :
        d && l ? 7 :
        d && r ? 4 :
        l ? 35 :
        r ? 33 :
        u ? 25 :
        d ? 3 :
        36;
    }
  }

  auto m = v::map_buffer();
  for (auto y = 0; y < 8; y++) {
    for (auto x = 0; x < 8; x++) {
      auto c = g_map[y][x];
      if (!c) continue;
      m += { .pos { 2 + x, 2 + y }, .uv = uv("environment/walls/brick_clay", c) };
    }
  }
  m += { .uv = uv("environment/props/mushroom1") };
  v::on_frame = [] {};
}

const int i = [] {
  v::on_frame = on_frame;
  return 0;
}();
