#pragma leco add_impl map_builder
#pragma leco add_impl map_walls
export module map;
import dotz;

export struct map {
  static constexpr const auto wall = 0x400'0000;

  static constexpr const unsigned w = 32;
  static constexpr const unsigned h = 24;

  unsigned data[h][w] {};

  void build();

  void load(auto m) {
    for (auto y = 0; y < h; y++) {
      for (auto x = 0; x < w; x++) {
        auto c = data[y][x];
        dotz::ivec2 uv { c % 64, c / 64 };
        m->push({ .pos { x, y }, .uv = uv });
      }
    }
  }
};
