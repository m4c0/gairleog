export module wallbuilder;
import rng;

constexpr auto rnd_rl() {
  switch (rng::rand(30)) {
    case 0: return 44;
    case 1: return 45;
    case 2: return 46;
    case 3: return 47;
    default: return 41;
  }
}

namespace wallbuilder {
  export template<unsigned W, unsigned H>
  void draw(unsigned (&map)[H][W]) {
    for (auto y = 0; y < H; y++) {
      for (auto x = 0; x < W; x++) {
        if (map[y][x] == 0) continue;

        bool l = x > 0   && map[y][x - 1];
        bool r = x < W-1 && map[y][x + 1];
        bool u = y > 0   && map[y - 1][x];
        bool d = y < H-1 && map[y + 1][x];
        map[y][x] =
          l && r && u && d ? 19 :
          l && r && u ? rnd_rl() :
          l && r && d ? 19 :
          l && u && d ? 18 :
          r && u && d ? 15 :
          l && r ? rnd_rl() :
          u && l ? 24 :
          u && r ? 22 :
          u && d ? 14 :
          d && l ? 7 :
          d && r ? 4 :
          l ? 35 :
          r ? 33 :
          u ? 25 :
          d ? 3 :
          36;
      }
    }
  }
}
