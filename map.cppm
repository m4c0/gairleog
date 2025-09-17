#pragma leco add_impl map_builder
#pragma leco add_impl map_walls
export module map;

export struct map {
  static constexpr const auto wall = 0x400'0000;

  static constexpr const unsigned w = 32;
  static constexpr const unsigned h = 24;

  unsigned data[h][w] {};

  void build();
};
