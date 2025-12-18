export module enemies;
import dotz;
import ents;
import hitdefs;
import pathing;
import rng;

namespace enemies {
  static dotz::ivec2 next_move(const ents::t & e, const pathing::t & m, dotz::ivec2 player) {
    auto step = m(e.pos);
    if (step.x || step.y) return step;

    auto d = e.pos - player;
    if (dotz::length(d) > 4) {
      switch (rng::rand(4)) {
        case 0: return { -1, 0 };
        case 1: return { 1, 0 };
        case 2: return { 0, -1 };
        case 3: return { 0, 1 };
        default: return {};
      }
    }

    auto ad = dotz::abs(d);
    if (ad.x > ad.y) {
      return { -dotz::sign(d.x), 0 };
    } else {
      return { 0, -dotz::sign(d.y )};
    }
  }

  export void tick() {
    dotz::ivec2 player {};
    ents::foreach({ .player = true }, [&](auto & p) {
      player = p.pos;
    });

    pathing::t m {};

    ents::foreach({ .enemy = true }, [&](auto & p) {
      ents::move(&p, next_move(p, m, player));
    });
  }
}
