export module enemies;
import dotz;
import ents;
import hitdefs;
import rng;

namespace enemies {
  export void tick() {
    ents::foreach([&](auto & p) {
      if (!p.flags.enemy) return;

      auto move = [] -> dotz::ivec2 {
        switch (rng::rand(4)) {
          case 0: return { -1, 0 };
          case 1: return { 1, 0 };
          case 2: return { 0, -1 };
          case 3: return { 0, 1 };
          default: return {};
        }
      }();
      ents::move(&p, move);
    });
  }
}
