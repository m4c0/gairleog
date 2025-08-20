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

  /*
    voo::memiter<sprite> m { *g_as->buf.memory, &g_as->count };
    m += { .pos { 2, 1 }, .uv = uv("environment/walls/brick_clay", 3) };
    m += { .pos { 2, 2 }, .uv = uv("environment/walls/brick_clay", 14) };
    m += { .pos { 2, 3 }, .uv = uv("environment/walls/brick_clay", 19) };
    m += { .pos { 2, 4 }, .uv = uv("environment/walls/brick_clay", 25) };
    m += { .pos { 1, 3 }, .uv = uv("environment/walls/brick_clay", 33) };
    m += { .pos { 3, 3 }, .uv = uv("environment/walls/brick_clay", 34) };
    m += { .pos { 4, 3 }, .uv = uv("environment/walls/brick_clay", 35) };
    m += { .uv = uv("environment/props/mushroom1") };
    */
