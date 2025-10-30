export module lootfx;
import hai;
import jute;

namespace lootfx {
  export struct fx {
    jute::heap name {};
  };

  hai::array<fx> map { 1024 };

  export bool has(int s) { return map[s].name.size() > 0; }

  export fx & get(int s) { return map[s]; }

  export void pick(int s) {
    auto & m = get(s);
    if (m.name.size()) return;
    m.name = "Yga";
  }
}
