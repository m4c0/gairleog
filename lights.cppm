export module lights;
import dotz;
import ents;
import map;

namespace lights {
  struct t {
    float emission;
    float current;
    bool solid;
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

  static constexpr const auto min_light = 0.3f;

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
    for (auto & row : data) {
      for (auto & c : row) {
        c.emission = 0;
        c.solid = false;
      }
    }

    ents::foreach([](const auto & e) {
      if (e.light) at(e.pos).emission = e.light * 3.0;
      if (e.flags.solid) at(e.pos).solid = true;
    });

    for (dotz::ivec2 p {}; p.y < map::h; p.y++) {
      for (p.x = 0; p.x < map::h; p.x++) {
        at(p).current = dotz::clamp(neighbour(p), 0.0f, 1.0f);
      }
    }
    for (dotz::ivec2 p {}; p.y < map::h; p.y++) {
      for (p.x = 0; p.x < map::h; p.x++) {
        auto & pp = at(p);
        if (!pp.solid) continue;
        if (pp.current > pp.emission || pp.current > min_light) pp.emission = pp.current;
      }
    }
  }
}
