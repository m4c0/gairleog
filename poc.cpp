#pragma leco app

import dotz;
import jute;
import sprdef;
import v;
import wallbuilder;

[[nodiscard]] constexpr auto uv(unsigned i) {
  return dotz::ivec2 { i % 64, i / 64 };
}
[[nodiscard]] auto uv(jute::view id, unsigned idx = 0) {
  static const auto sprdefs = sprdef::load();
  return uv(sprdefs[id] + idx);
}

static unsigned g_map[8][12] {
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1 },
  { 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1 },
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

static void on_frame() {
  wallbuilder::draw(g_map);

  auto m = v::map_buffer();
  for (auto y = 0; y < 8; y++) {
    for (auto x = 0; x < 12; x++) {
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
