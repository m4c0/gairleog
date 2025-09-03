export module mapbuilder;
import dotz;
import rng;
import roombuilder;

namespace mapbuilder {
  template<unsigned W, unsigned H>
  void vsplit(unsigned (&map)[H][W], dotz::ivec2 aa, dotz::ivec2 bb);

  template<unsigned W, unsigned H>
  void hsplit(unsigned (&map)[H][W], dotz::ivec2 aa, dotz::ivec2 bb) {
    if (bb.x - aa.x + 1 <= 2) return;
    if (roombuilder::supports(bb - aa)) return;

    auto x = aa.x + 1;
    if (bb.x - aa.x - 1 > 1) x += rng::rand(bb.x - aa.x - 1);

    for (auto y = aa.y; y <= bb.y; y++) map[y][x] = 1;

    vsplit(map, aa, {x-1, bb.y});
    vsplit(map, {x+1, aa.y}, bb);
 
    auto y = aa.y;
    if (bb.y - aa.y > 1) y += rng::rand(bb.y - aa.y);
    map[y][x-1] = 0;
    map[y][x] = 0;
    map[y][x+1] = 0;
  }

  template<unsigned W, unsigned H>
  void vsplit(unsigned (&map)[H][W], dotz::ivec2 aa, dotz::ivec2 bb) {
    if (bb.y - aa.y + 1 <= 2) return;
    if (roombuilder::supports(bb - aa)) return;

    auto y = aa.y + 1;
    if (bb.y - aa.y - 1 > 1) y += rng::rand(bb.y - aa.y - 1);

    for (auto x = aa.x; x <= bb.x; x++) map[y][x] = 1;

    hsplit(map, aa, {bb.x, y-1});
    hsplit(map, {aa.x, y+1}, bb);
 
    auto x = aa.x;
    if (bb.x - aa.x > 1) x += rng::rand(bb.x - aa.x);
    map[y-1][x] = 0;
    map[y][x] = 0;
    map[y+1][x] = 0;
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
