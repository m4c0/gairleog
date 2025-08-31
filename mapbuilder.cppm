export module mapbuilder;
import dotz;
import rng;

namespace mapbuilder {
  template<unsigned W, unsigned H>
  void hsplit(unsigned (&map)[H][W], dotz::ivec2 aa, dotz::ivec2 bb) {
    auto x = 0;
    do {
      x = rng::rand(bb.x - aa.x + 1) + aa.x;
    } while (!map[aa.y - 1][x] || !map[bb.y + 1][x]);

    for (auto y = aa.y; y <= bb.y; y++) map[y][x] = 1;
    map[rng::rand(bb.y - aa.y + 1) + aa.y][x] = 0;
  }

  template<unsigned W, unsigned H>
  void vsplit(unsigned (&map)[H][W], dotz::ivec2 aa, dotz::ivec2 bb) {
    auto y = 0;
    do {
      y = rng::rand(bb.y - aa.y + 1) + aa.y;
    } while (!map[y][aa.x - 1] || !map[y][bb.x + 1]);

    for (auto x = aa.x; x <= bb.x; x++) map[y][x] = 1;
    map[y][rng::rand(bb.x - aa.x + 1) + aa.x] = 0;
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
