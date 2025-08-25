export module mapbuilder;

namespace mapbuilder {
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
  }
}
