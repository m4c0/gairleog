export module lights;
import dotz;
import ents;
import map;

namespace lights {
  struct t {
    float emission;
    float current;
  };

  t data[map::h][map::w] {};

  export auto & at(dotz::ivec2 p) {
    return data[p.y][p.x];
  }

  export void reset() {
    for (auto & row : data)
      for (auto & c : row)
        c = {};
  }

  static float neighbour(dotz::ivec2 center) {
    float res = 0;

    for (auto p = center - 1; p.y <= center.y + 1; p.y++) {
      if (p.y < 0 || p.y >= map::h) continue;
      for (p.x = center.x - 1; p.x <= center.x + 1; p.x++) {
        if (p.x < 0 || p.x >= map::w) continue;
        float le = at(p).emission;
        float lc = at(p).current;
        float den = (p.x == 0 && p.y == 0) 
          ? 1.0
          : (1.0 + 0.25 * dotz::length(dotz::vec2 { p - center }));
        res += dotz::max(le, lc) / den;
      }
    }

    return res / 9.0f;
  }

  export void tick() {
    for (auto & row : data)
      for (auto & c : row)
        c.emission = 0;

    ents::foreach([](const auto & e) {
      if (e.light) at(e.pos).emission = e.light * 1.0;
    });

    for (dotz::ivec2 p {}; p.y < map::h; p.y++) {
      for (p.x = 0; p.x < map::h; p.x++) {
        at(p).current = dotz::clamp(neighbour(p), 0.0f, 1.0f);
      }
    }

    ents::foreach([](auto & e) {
      auto l = dotz::max(at(e.pos).current, e.light);
      if (e.flags.wall || e.flags.ceramic) e.light = dotz::clamp(l, 0.0f, 0.1f);
    });
  }
}
