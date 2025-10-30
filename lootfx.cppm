#pragma leco add_impl lootfx_impl
export module lootfx;
import hai;
import jute;
import rng;

namespace lootfx {
  export struct fx {
    jute::heap name {};
  };

  hai::array<fx> map { 4096 };
  hai::varray<fx> rest { 16 };

  export void reset() {
    for (auto & m : map) m = {};

    rest.push_back(fx { .name = "Heal" });
    rest.push_back(fx { .name = "Poison" });
  }

  export bool has(int s) { return map[s].name.size() > 0; }

  export const fx & get(int s) { return map[s]; }

  export void run(jute::view);

  export void pick(int s) {
    if (has(s)) return;
    if (rest.size() == 0) return;

    auto n = rng::rand(rest.size());
    map[s] = rest[n];
    rest[n] = rest.pop_back();
  }
}
