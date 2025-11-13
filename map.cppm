#pragma leco add_impl map_builder
#pragma leco add_impl map_lights
#pragma leco add_impl map_walls
export module map;
import dotz;
import entdefs;

struct tile : entdefs::t {
  float clight {};
};

export struct map {
  static constexpr const unsigned w = 32;
  static constexpr const unsigned h = 32;

  tile data[h][w] {};

  auto & at(dotz::ivec2 p) { return data[p.y][p.x]; }

  void build();

  void foreach(auto && fn) {
    for (auto y = 0; y < h; y++) {
      for (auto x = 0; x < w; x++) {
        fn(dotz::ivec2 { x, y }, data[y][x]);
      }
    }
  }

  void tick_lights(dotz::ivec2 p, float l, float ms);
};
