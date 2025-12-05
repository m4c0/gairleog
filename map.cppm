#pragma leco add_impl map_builder
#pragma leco add_impl map_walls
export module map;
import entdefs;

namespace map {
  export constexpr const unsigned w = 32;
  export constexpr const unsigned h = 32;

  using t = entdefs::t[h][w];
  export void build();
};
