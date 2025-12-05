#pragma leco add_impl map_builder
#pragma leco add_impl map_walls
export module map;
import dotz;
import entdefs;

namespace map {
  export constexpr const unsigned w = 32;
  export constexpr const unsigned h = 32;

  struct t {
    entdefs::t data[h][w] {};

    auto & at(dotz::ivec2 p) { return data[p.y][p.x]; }

    void build();
  };
  export void build() {
    t {}.build();
  }
};
