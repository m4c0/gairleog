export module mapbuilder;
import dotz;
import rng;
import roomdefs;

namespace mapbuilder {
  export
  template<unsigned W, unsigned H>
  struct map {
    unsigned data[H][W];
    roomdefs::list roomdefs;
  };

  template<unsigned W, unsigned H>
  [[nodiscard]] bool furnish(map<W, H> & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    return false;
  }

  template<unsigned W, unsigned H>
  void vsplit(map<W, H> & map, dotz::ivec2 aa, dotz::ivec2 bb);

  template<unsigned W, unsigned H>
  void hsplit(map<W, H> & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    if (bb.x - aa.x + 1 <= 2) return;
    if (furnish(map, aa, bb)) return;

    auto x = aa.x + 1;
    if (bb.x - aa.x - 1 > 1) x += rng::rand(bb.x - aa.x - 1);

    for (auto y = aa.y; y <= bb.y; y++) map.data[y][x] = 1;

    vsplit(map, aa, {x-1, bb.y});
    vsplit(map, {x+1, aa.y}, bb);
 
    auto y = aa.y;
    if (bb.y - aa.y > 1) y += rng::rand(bb.y - aa.y);
    map.data[y][x-1] = 0;
    map.data[y][x] = 0;
    map.data[y][x+1] = 0;
  }

  template<unsigned W, unsigned H>
  void vsplit(map<W, H> & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    if (bb.y - aa.y + 1 <= 2) return;
    if (furnish(map, aa, bb)) return;

    auto y = aa.y + 1;
    if (bb.y - aa.y - 1 > 1) y += rng::rand(bb.y - aa.y - 1);

    for (auto x = aa.x; x <= bb.x; x++) map.data[y][x] = 1;

    hsplit(map, aa, {bb.x, y-1});
    hsplit(map, {aa.x, y+1}, bb);
 
    auto x = aa.x;
    if (bb.x - aa.x > 1) x += rng::rand(bb.x - aa.x);
    map.data[y-1][x] = 0;
    map.data[y][x] = 0;
    map.data[y+1][x] = 0;
  }

  export
  template<unsigned W, unsigned H>
  void build(map<W, H> & map) {
    for (auto y = 0; y < H; y++) {
      for (auto x = 0; x < W; x++) {
        map.data[y][x] = 0;
      }
    }
    for (auto y = 0; y < H; y++) map.data[y][0] = map.data[y][W - 1] = 1;
    for (auto x = 0; x < W; x++) map.data[0][x] = map.data[H - 1][x] = 1;

    if (W > H) {
      vsplit(map, {1,1}, {W-2,H-2});
    } else {
      hsplit(map, {1,1}, {W-2,H-2});
    }
  }
}
