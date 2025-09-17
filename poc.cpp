#pragma leco app

import casein;
import dotz;
import jute;
import hai;
import hashley;
import map;
import roomdefs;
import silog;
import sires;
import sprdef;
import tiledefs;
import traits;
import v;

[[nodiscard]] constexpr auto uv(unsigned i) {
  return dotz::ivec2 { i % 64, i / 64 };
}
[[nodiscard]] auto uv(jute::view id, unsigned idx = 0) {
  return uv(sprdef::get(id) + idx);
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
  sires::on_error([](auto ptr, auto msg) { silog::die("Failure loading resource: %s", msg.cstr().begin()); });
  sprdef::load([] {
    tiledefs::load([] {
      roomdefs::load([](auto rooms) {
        g_map.roomdefs = traits::move(rooms);

        v::pc = { 16, 16 };
        v::on_frame = on_frame;
      });
    });
  });
  return 0;
}();
