export module mapbuilder;
import dotz;
import map;
import rng;

namespace mapbuilder {
  [[nodiscard]] bool furnish(map & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    return false;
  }

  void vsplit(map & map, dotz::ivec2 aa, dotz::ivec2 bb);

  void hsplit(map & map, dotz::ivec2 aa, dotz::ivec2 bb) {
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

  void vsplit(map & map, dotz::ivec2 aa, dotz::ivec2 bb) {
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

  export void build(map & map) {
    for (auto y = 0; y < map.h; y++) {
      for (auto x = 0; x < map.w; x++) {
        map.data[y][x] = 0;
      }
    }
    for (auto y = 0; y < map.h; y++) map.data[y][0] = map.data[y][map.w - 1] = 1;
    for (auto x = 0; x < map.w; x++) map.data[0][x] = map.data[map.h - 1][x] = 1;

    (map.w > map.h ? vsplit : hsplit)(map, {1,1}, {map.w-2,map.h-2});
  }
}
