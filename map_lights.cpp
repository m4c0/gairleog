module map;
import traits;

static constexpr const auto min_light = 0.3f;

static float neighbour(map & m, dotz::ivec2 center) {
  float res = 0;

  for (auto p = center - 1; p.y <= center.y + 1; p.y++) {
    if (p.y < 0 || p.y >= map::h) continue;
    for (p.x = center.x - 1; p.x <= center.x + 1; p.x++) {
      if (p.x < 0 || p.x >= map::w) continue;
      float le = m.at(p).light;
      float lc = m.at(p).clight;
      float den = (p.x == 0 && p.y == 0) 
        ? 1.0
        : (1.0 + 0.25 * dotz::length(dotz::vec2 { p - center }));
      res += dotz::max(le, lc) / den;
    }
  }

  return res / 9.0f;
}

void map::tick_lights(dotz::ivec2 p, float l, float ms) {
  at(p).light = l * 3.0;

  for (dotz::ivec2 p {}; p.y < h; p.y++) {
    for (p.x = 0; p.x < h; p.x++) {
      at(p).clight = dotz::clamp(neighbour(*this, p), 0.0f, 1.0f);
    }
  }
  for (dotz::ivec2 p {}; p.y < h; p.y++) {
    for (p.x = 0; p.x < h; p.x++) {
      auto & pp = at(p);
      if (!pp.flags.solid) continue;
      if (pp.clight > pp.light || pp.clight > min_light) pp.light = pp.clight;
    }
  }

  at(p).light = 0;
}
