#pragma leco add_impl map_builder
#pragma leco add_impl map_walls
export module map;
import dotz;
import entdefs;

export struct map {
  static constexpr const unsigned w = 32;
  static constexpr const unsigned h = 32;

  entdefs::t data[h][w] {};

  auto & at(dotz::ivec2 p) { return data[p.y][p.x]; }

  void build();
};
