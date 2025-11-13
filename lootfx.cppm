#pragma leco add_impl lootfx_impl
export module lootfx;
import hai;
import jute;
import rng;

namespace lootfx {
  export enum class action {
    defence,
    heal,
    max_life,
    strength,
    weakness,
    wither,
  };
  export using action_list_t = hai::chain<action>;

  hai::array<jute::view> map { 4096 };
  hai::varray<jute::view> rest { 16 };

  export void reset();

  export bool has(int s) { return map[s].size() > 0; }

  export auto get(int s) { return map[s]; }

  export void run(jute::view);

  void apply(jute::view key, action_list_t * result);
  void pick(int s) {
    if (has(s)) return;
    if (rest.size() == 0) return;

    auto n = rng::rand(rest.size());
    map[s] = rest[n];
    rest[n] = rest.pop_back();
  }
  export action_list_t apply(int s) {
    action_list_t result { 8 };
    pick(s);
    apply(get(s), &result);
    return result;
  }
}
