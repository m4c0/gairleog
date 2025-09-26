module map;
import traits;

static float neighbour(map & m, dotz::ivec2 center) {
  float res = 0;

  for (auto p = center - 1; p.y <= center.y + 1; p.y++) {
    if (p.y < 0 || p.y >= map::h) continue;
    for (p.x = center.x - 1; p.x <= center.x + 1; p.x++) {
      if (p.x < 0 || p.x >= map::w) continue;
      float den = (p.x == 0 && p.y == 0) 
        ? 1.0
        : (1.0 + 0.25 * dotz::length(dotz::vec2 { p - center }));
      res += m.at(p).def.light / den;
    }
  }

  return res / 9.0f;
}

void map::tick_lights(dotz::ivec2 p, float l, float ms) {
  at(p).def.light = l * 3.0;

  map bb {};
  for (dotz::ivec2 p {}; p.y < h; p.y++) {
    for (p.x = 0; p.x < h; p.x++) {
      bb.at(p) = at(p);
      bb.at(p).def.light = dotz::clamp(neighbour(*this, p), 0.0f, 1.0f);
    }
  }
  *this = traits::move(bb);
}
