#pragma leco add_impl map_builder
#pragma leco add_impl map_lights
#pragma leco add_impl map_walls
export module map;
import dotz;
import roomdefs;

struct tile {
  roomdefs::tiledef def {};
  float light {};
  bool exit :1 {};
};

export struct map {
  static constexpr const unsigned w = 32;
  static constexpr const unsigned h = 32;

  tile data[h][w] {};

  auto & at(dotz::ivec2 p) { return data[p.y][p.x]; }

  void build();

  void load(auto && m) {
    for (auto y = 0; y < h; y++) {
      for (auto x = 0; x < w; x++) {
        m->push({
          .pos { x, y },
          .mult = data[y][x].light,
          .id = data[y][x].def.sprite,
        });
      }
    }
  }

  void tick_lights(dotz::ivec2 p, float l, float ms);
};
