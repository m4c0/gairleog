#pragma leco add_impl lootfx_impl
export module lootfx;
import hai;
import jute;
import rng;

namespace lootfx {
  hai::array<jute::view> map { 4096 };
  hai::varray<jute::view> rest { 16 };

  export void reset();

  export bool has(int s) { return map[s].size() > 0; }

  export auto get(int s) { return map[s]; }

  export void run(jute::view);

  export void pick(int s) {
    if (has(s)) return;
    if (rest.size() == 0) return;

    auto n = rng::rand(rest.size());
    map[s] = rest[n];
    rest[n] = rest.pop_back();
  }
}
