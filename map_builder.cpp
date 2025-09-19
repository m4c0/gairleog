module map;
import dotz;
import rng;
import roomdefs;
import sprdef;

namespace {
  static constexpr const auto max_room_size = roomdefs::max_size;

  [[nodiscard]] bool furnish(map & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    auto ab = bb - aa + 1;
    if (ab.x > max_room_size || ab.y > max_room_size) return false;

    auto n = roomdefs::for_size(ab.x, ab.y);
    if (!n) return false;
    for (auto y = aa.y; y <= bb.y; y++) {
      for (auto x = aa.x; x <= bb.x; x++) {
        map.data[y][x] = n->data[(y - aa.y) * n->w + (x - aa.x)];
      }
    }

    return true;
  }

  void vsplit(map & map, dotz::ivec2 aa, dotz::ivec2 bb);

  void hsplit(map & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    if (furnish(map, aa, bb)) return;
    if (bb.x - aa.x + 1 <= 2) return;

    int x = rng::rand_i(aa.x + 1, bb.x - 1);

    for (auto y = aa.y; y <= bb.y; y++) map.data[y][x] = map::wall;

    vsplit(map, aa, {x-1, bb.y});
    vsplit(map, {x+1, aa.y}, bb);
 
    auto y = rng::rand_i(aa.y, bb.y);
    for (auto i = 0; i < bb.y - aa.y; i++) {
      if (!map.data[y][x - 1].block &&
          !map.data[y][x + 1].block) {
        map.data[y][x] = {};
        break;
      }

      y++;
      if (y > bb.y) y = aa.y;
    }
    map.data[y][x-1] = {};
    map.data[y][x] = {};
    map.data[y][x+1] = {};
  }

  void vsplit(map & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    if (furnish(map, aa, bb)) return;
    if (bb.y - aa.y + 1 <= 2) return;

    int y = rng::rand_i(aa.y + 1, bb.y - 1);

    for (auto x = aa.x; x <= bb.x; x++) map.data[y][x] = map::wall;

    hsplit(map, aa, {bb.x, y-1});
    hsplit(map, {aa.x, y+1}, bb);
 
    auto x = rng::rand_i(aa.x, bb.x);
    for (auto i = 0; i < bb.x - aa.x; i++) {
      if (!map.data[y - 1][x].block &&
          !map.data[y + 1][x].block) {
        map.data[y][x] = {};
        break;
      }

      x++;
      if (x > bb.x) x = aa.x;
    }
    map.data[y-1][x] = {};
    map.data[y][x] = {};
    map.data[y+1][x] = {};
  }
}

void make_walls(map & map);

void map::build() {
  for (auto y = 0; y < h; y++) {
    for (auto x = 0; x < w; x++) {
      data[y][x] = {};
    }
  }
  for (auto y = 0; y < h; y++) data[y][0] = data[y][w - 1] = map::wall;
  for (auto x = 0; x < w; x++) data[0][x] = data[h - 1][x] = map::wall;

  (w > h ? vsplit : hsplit)(*this, {1,1}, {w-2,h-2});
  make_walls(*this);

  auto base = sprdef::get("environment/walls/brick_clay");
  for (auto y = 0; y < h; y++) {
    for (auto x = 0; x < w; x++) {
      auto & c = data[y][x].sprite;
      if (c & wall_id) c = base + (c & 0xFFFF);
    }
  }
}
