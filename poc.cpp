#pragma leco app

import casein;
import dotz;
import jute;
import hai;
import hashley;
import map;
import res;
import silog;
import sires;
import traits;
import v;

[[nodiscard]] constexpr auto uv(unsigned i) {
  return dotz::ivec2 { i % 64, i / 64 };
}

static map g_map {};

static void load(auto m) {
  for (auto y = 0; y < g_map.h; y++) {
    for (auto x = 0; x < g_map.w; x++) {
      auto c = g_map.data[y][x];
      m->push({ .pos { x, y }, .uv = uv(c) });
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
  res::on_error([](auto msg) { silog::die("Failure loading resource: %s", msg.cstr().begin()); });
  res::load_all([] {
    v::pc = { 16, 16 };
    v::on_frame = on_frame;
  });
  return 0;
}();
