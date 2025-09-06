#pragma leco add_impl map_builder
export module map;
import roomdefs;

export struct map {
  static constexpr const unsigned w = 32;
  static constexpr const unsigned h = 24;

  unsigned data[h][w] {};
  roomdefs::list roomdefs {};

  void build();
};
