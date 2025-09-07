module map;
import dotz;
import rng;

namespace {
  [[nodiscard]] bool furnish(map & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    auto ab = bb - aa + 1;
    if (ab.x > map::max_room_size || ab.y > map::max_room_size) return false;

    auto & r = map.roomdefs.data[ab.y - 1][ab.x - 1];
    if (r.size() == 0) return false;

    auto & n = r[rng::rand(r.size())];
    for (auto y = aa.y; y <= bb.y; y++) {
      for (auto x = aa.x; x <= bb.x; x++) {
        map.data[y][x] = n->data[(y - aa.y) * n->w + (x - aa.x)];
      }
    }

    return true;
  }

  void vsplit(map & map, dotz::ivec2 aa, dotz::ivec2 bb);

  void hsplit(map & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    if (bb.x - aa.x + 1 <= 2) return;
    if (furnish(map, aa, bb)) return;

    auto x = aa.x + 1;
    if (bb.x - aa.x - 1 > 1) x += rng::rand(bb.x - aa.x - 1);

    for (auto y = aa.y; y <= bb.y; y++) map.data[y][x] = map::wall;

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

    for (auto x = aa.x; x <= bb.x; x++) map.data[y][x] = map::wall;

    hsplit(map, aa, {bb.x, y-1});
    hsplit(map, {aa.x, y+1}, bb);
 
    auto x = aa.x;
    if (bb.x - aa.x > 1) x += rng::rand(bb.x - aa.x);
    map.data[y-1][x] = 0;
    map.data[y][x] = 0;
    map.data[y+1][x] = 0;
  }
}

void map::build() {
  for (auto y = 0; y < h; y++) {
    for (auto x = 0; x < w; x++) {
      data[y][x] = 0;
    }
  }
  for (auto y = 0; y < h; y++) data[y][0] = data[y][w - 1] = map::wall;
  for (auto x = 0; x < w; x++) data[0][x] = data[h - 1][x] = map::wall;

  (w > h ? vsplit : hsplit)(*this, {1,1}, {w-2,h-2});
}
