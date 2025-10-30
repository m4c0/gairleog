export module lootfx;
import hai;
import jute;

namespace lootfx {
  export struct fx {
    jute::heap name {};
  };

  hai::array<fx> map { 4096 };

  export bool has(int s) { return map[s].name.size() > 0; }

  export const fx & get(int s) { return map[s]; }

  export void pick(int s) {
    if (has(s)) return;

    auto & m = map[s];
    m.name = "+\3-str";
  }
}
