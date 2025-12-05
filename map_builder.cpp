module map;
import dotz;
import entdefs;
import perlin;
import rng;
import roomdefs;
import sprdef;

namespace {
  // TODO: move to lisp
  static constexpr const entdefs::t wall {
    .flags {
      .fluorescent = true,
      .solid = true,
      .wall = true,
    },
  };

  [[nodiscard]] bool furnish(map & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    auto ab = bb - aa + 1;
    auto n = roomdefs::for_size(ab.x, ab.y);
    if (!n) return false;
    for (auto y = aa.y; y <= bb.y; y++) {
      for (auto x = aa.x; x <= bb.x; x++) {
        map.data[y][x] = { n->fn(*n, x - aa.x, y - aa.y) };
      }
    }

    return true;
  }

  void vsplit(map & map, dotz::ivec2 aa, dotz::ivec2 bb);

  void hsplit(map & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    if (furnish(map, aa, bb)) return;
    if (bb.x - aa.x + 1 <= 2) return;

    int x = rng::rand_i(aa.x + 1, bb.x - 1);

    for (auto y = aa.y; y <= bb.y; y++) map.data[y][x] = wall;

    vsplit(map, aa, {x-1, bb.y});
    vsplit(map, {x+1, aa.y}, bb);
 
    auto y = rng::rand_i(aa.y, bb.y);
    for (auto i = 0; i < bb.y - aa.y; i++) {
      if (!map.data[y][x - 1].flags.solid &&
          !map.data[y][x + 1].flags.solid) {
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

    for (auto x = aa.x; x <= bb.x; x++) map.data[y][x] = wall;

    hsplit(map, aa, {bb.x, y-1});
    hsplit(map, {aa.x, y+1}, bb);
 
    auto x = rng::rand_i(aa.x, bb.x);
    for (auto i = 0; i < bb.x - aa.x; i++) {
      if (!map.data[y - 1][x].flags.solid &&
          !map.data[y + 1][x].flags.solid) {
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

void make_walls(perlin & pln, map & map);

void map::build() {
  for (auto y = 0; y < h; y++) {
    for (auto x = 0; x < w; x++) {
      data[y][x] = {};
    }
  }
  for (auto y = 0; y < h; y++) data[y][0] = data[y][w - 1] = wall;
  for (auto x = 0; x < w; x++) data[0][x] = data[h - 1][x] = wall;

  (w > h ? vsplit : hsplit)(*this, {1,1}, {w-2,h-2});

  perlin pln {};
  make_walls(pln, *this);

  const unsigned bases[] {
    sprdef::get("environment/walls/brick_clay").id,
    sprdef::get("environment/walls/walls_cave_brown").id,
    sprdef::get("environment/walls/walls_cave_gray").id,
    sprdef::get("environment/walls/walls_dungeon_gray").id,
    sprdef::get("environment/walls/walls_dungeon_yellow").id,
  };
  for (auto y = 0; y < h; y++) {
    for (auto x = 0; x < w; x++) {
      if (!data[y][x].flags.wall) continue;
      dotz::vec2 p { static_cast<float>(x) / w, static_cast<float>(y) / h };
      data[y][x].sprite += bases[static_cast<unsigned>(pln(p) * 2.5 + 2.5)];
    }
  }

  data[1][1] = { entdefs::get("player") };
  data[h - 2][w - 2] = { entdefs::get("exit") };
}
