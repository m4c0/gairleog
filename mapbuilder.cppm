export module mapbuilder;
import dotz;

namespace mapbuilder {
  template<unsigned W, unsigned H>
  void hsplit(unsigned (&map)[H][W], dotz::ivec2 aa, dotz::ivec2 bb) {
  }

  template<unsigned W, unsigned H>
  void vsplit(unsigned (&map)[H][W], dotz::ivec2 aa, dotz::ivec2 bb) {
  }

  export
  template<unsigned W, unsigned H>
  void build(unsigned (&map)[H][W]) {
    for (auto y = 0; y < H; y++) {
      for (auto x = 0; x < W; x++) {
        map[y][x] = 0;
      }
    }
    for (auto y = 0; y < H; y++) map[y][0] = map[y][W - 1] = 1;
    for (auto x = 0; x < W; x++) map[0][x] = map[H - 1][x] = 1;

    if (W > H) {
      vsplit(map, {1,1}, {W-2,H-2});
    } else {
      hsplit(map, {1,1}, {W-2,H-2});
    }
  }
}
