export module enemies;
import dotz;
import ents;
import hitdefs;
import rng;

namespace enemies {
  static dotz::ivec2 next_move(const ents::t & e, dotz::ivec2 player) {
    switch (rng::rand(4)) {
      case 0: return { -1, 0 };
      case 1: return { 1, 0 };
      case 2: return { 0, -1 };
      case 3: return { 0, 1 };
      default: return {};
    }
  }

  export void tick() {
    dotz::ivec2 player {};
    ents::foreach([&](auto & p) {
      if (p.flags.player) player = p.pos;
    });

    ents::foreach([&](auto & p) {
      if (!p.flags.enemy) return;

      ents::move(&p, next_move(p, player));
    });
  }
}
