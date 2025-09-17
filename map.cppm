#pragma leco add_impl map_builder
#pragma leco add_impl map_walls
export module map;
import dotz;
import roomdefs;

export struct map {
  static constexpr const auto wall_id = 0x400'0000;

  static constexpr const roomdefs::tiledef wall {
    .block = true,
    .sprite = wall_id,
  };

  static constexpr const unsigned w = 32;
  static constexpr const unsigned h = 24;

  roomdefs::tiledef data[h][w] {};

  auto & at(dotz::ivec2 p) { return data[p.y][p.x]; }

  void build();

  void load(auto && m) {
    for (auto y = 0; y < h; y++) {
      for (auto x = 0; x < w; x++) {
        m->push({
          .pos { x, y },
          .id = data[y][x].sprite,
        });
      }
    }
  }
};
