module map;
import dotz;
import ents;
import entdefs;
import hai;
import rng;
import roomdefs;
import save;
import sprdef;

namespace {
  static auto wall() { return entdefs::get("wall"); }

  [[nodiscard]] bool furnish(map::t & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    auto ab = bb - aa + 1;
    auto n = roomdefs::for_size(ab.x, ab.y);
    if (!n) return false;
    for (auto y = aa.y; y <= bb.y; y++) {
      for (auto x = aa.x; x <= bb.x; x++) {
        map[y][x] = { n->fn(*n, x - aa.x, y - aa.y) };
      }
    }

    return true;
  }

  void vsplit(map::t & map, dotz::ivec2 aa, dotz::ivec2 bb);

  void hsplit(map::t & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    if (furnish(map, aa, bb)) return;
    if (bb.x - aa.x + 1 <= 2) return;

    int x = rng::rand_i(aa.x + 1, bb.x - 1);

    for (auto y = aa.y; y <= bb.y; y++) map[y][x] = wall();

    vsplit(map, aa, {x-1, bb.y});
    vsplit(map, {x+1, aa.y}, bb);
 
    auto y = rng::rand_i(aa.y, bb.y);
    for (auto i = 0; i < bb.y - aa.y; i++) {
      if (!map[y][x - 1].flags.solid &&
          !map[y][x + 1].flags.solid) {
        map[y][x] = {};
        break;
      }

      y++;
      if (y > bb.y) y = aa.y;
    }
    map[y][x-1] = {};
    map[y][x] = {};
    map[y][x+1] = {};
  }

  void vsplit(map::t & map, dotz::ivec2 aa, dotz::ivec2 bb) {
    if (furnish(map, aa, bb)) return;
    if (bb.y - aa.y + 1 <= 2) return;

    int y = rng::rand_i(aa.y + 1, bb.y - 1);

    for (auto x = aa.x; x <= bb.x; x++) map[y][x] = wall();

    hsplit(map, aa, {bb.x, y-1});
    hsplit(map, {aa.x, y+1}, bb);
 
    auto x = rng::rand_i(aa.x, bb.x);
    for (auto i = 0; i < bb.x - aa.x; i++) {
      if (!map[y - 1][x].flags.solid &&
          !map[y + 1][x].flags.solid) {
        map[y][x] = {};
        break;
      }

      x++;
      if (x > bb.x) x = aa.x;
    }
    map[y-1][x] = {};
    map[y][x] = {};
    map[y+1][x] = {};
  }
}

void make_walls(map::t & map);

void map::build() {
  // Hack to use heap for this. On WASM this overflows the stack.
  struct mapt { map::t data; };
  auto dptr = hai::uptr<mapt>::make();
  auto & data = dptr->data;

  for (auto y = 0; y < h; y++) {
    for (auto x = 0; x < w; x++) {
      data[y][x] = {};
    }
  }
  for (auto y = 0; y < h; y++) data[y][0] = data[y][w - 1] = wall();
  for (auto x = 0; x < w; x++) data[0][x] = data[h - 1][x] = wall();

  (w > h ? vsplit : hsplit)(data, {1,1}, {w-2,h-2});

  bool flip = save::current_stage % 2;
  auto px =  flip ? 1 : w - 2;
  auto ex = !flip ? 1 : w - 2;
  data[0][px] = { entdefs::get("entry") };
  data[1][px] = { entdefs::get("player") };
  data[h - 2][ex] = {};
  data[h - 1][ex] = { entdefs::get("exit") };

  make_walls(data);

  for (auto y = 0; y < h; y++) {
    for (auto x = 0; x < w; x++) {
      ents::add(dotz::ivec2 { x, y }, data[y][x]);
    }
  }
}
