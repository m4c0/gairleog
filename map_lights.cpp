module map;
import traits;

static float neighbour(map & m, dotz::ivec2 center) {
  float res = 0;
  float n = 0;

  for (auto p = center - 1; p.y <= center.y + 1; p.y++) {
    if (p.y < 0 || p.y >= map::h) continue;
    for (p.x = center.x - 1; p.x <= center.x + 1; p.x++) {
      if (p.x < 0 || p.x >= map::w) continue;
      float den = (p.x == 0 && p.y == 0) 
        ? 1.0
        : (1.0 + 0.25 * dotz::length(dotz::vec2 { p - center }));
      res += m.at(p).light / den;
      n += 1;
    }
  }

  return res / n;
}

void map::tick_lights(dotz::ivec2 p, float l, float ms) {
  at(p).light = l;

  map bb {};
  for (dotz::ivec2 p {}; p.y < h; p.y++) {
    for (p.x = 0; p.x < h; p.x++) {
      bb.at(p) = at(p);
      bb.at(p).light = neighbour(*this, p);
    }
  }
  *this = traits::move(bb);
}
