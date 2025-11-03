export module enemies;
import ents;

namespace enemies {
  export void tick() {
    ents::foreach([&](auto & p) {
      if (!p.flags.enemy) return;
    });
  }
}
