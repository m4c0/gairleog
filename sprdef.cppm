export module sprdef;
import glispy;
import hai;
import hashley;
import jute;
import sires;
import traits;

namespace sprdef {
  export struct pair {
    unsigned id;
    unsigned qty;
  };
  auto & map() {
    static hashley::fin<pair> i { 1023 };
    return i;
  }
  export bool has(jute::view key) { return map().has(key); }
  export const auto & get(jute::view key) { return map()[key]; }

  static auto sprdef(const lispy::node * n, const lispy::node * def, const lispy::node * val, const lispy::node * qty) {
    if (!lispy::is_atom(def)) lispy::erred(def, "def name must be an atom");
    map()[def->atom] = {
      .id  = lispy::to_u32(val),
      .qty = lispy::to_u32(qty),
    };
    return n;
  }
  export void run(jute::view fname, jute::view src) {
    lispy::temp_arena<lispy::node> a {};
    lispy::temp_frame ctx {};
    ctx.fns["sprdef"] = lispy::experimental::wrap<lispy::node, sprdef>;
    lispy::run<lispy::node>(fname, src);
  }
}
