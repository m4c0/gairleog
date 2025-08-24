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

static unsigned g_map[12][16] {
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1 },
  { 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1 },
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

template<unsigned W, unsigned H>
static void load(auto & m, unsigned (&map)[H][W]) {
  for (auto y = 0; y < H; y++) {
    for (auto x = 0; x < W; x++) {
      auto c = map[y][x];
      if (!c) continue;
      m += { .pos { x, y }, .uv = uv("environment/walls/brick_clay", c) };
    }
  }
}

static void on_frame() {
  wallbuilder::draw(g_map);

  auto m = v::map_buffer();
  load(m, g_map);
  v::on_frame = [] {};
}

const int i = [] {
  v::on_frame = on_frame;
  return 0;
}();
