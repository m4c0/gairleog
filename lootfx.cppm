#pragma leco add_impl lootfx_impl
#pragma leco add_impl lootfx_player
export module lootfx;
import file;
import hai;
import jute;
import rng;

namespace lootfx {
  export enum class outcome {
    none,
    death,
  };
  export enum class action {
    damage,
    defence,
    heal,
    max_life,
    poison,
    strength,
    weakness,
    wither,
  };
  using action_list_t = hai::chain<action>;

  hai::array<jute::heap> map { 4096 }; // Sprite ID to FX
  hai::varray<jute::heap> rest { 16 }; // List of available FX

  export void reset();

  export void read(file::reader * r);
  export void write(file::writer * w);

  export bool has(int s) { return map[s].size() > 0; }

  export auto get(int s) { return s < map.size() ? map[s] : ""; }

  export void run(jute::view);

  void apply(jute::view key, action_list_t * result);
  [[nodiscard]] outcome apply_on_player(const action_list_t & actions);
  void pick(int s) {
    if (has(s)) return;
    if (rest.size() == 0) return;

    auto n = rng::rand(rest.size());
    map[s] = rest[n];
    rest[n] = rest.pop_back();
  }
  export [[nodiscard]] outcome apply_by_name(jute::view name) {
    action_list_t result { 8 };
    apply(name, &result);
    return apply_on_player(result);
  }
  export [[nodiscard]] outcome apply(int s) {
    pick(s);
    return apply_by_name(get(s));
  }
}
