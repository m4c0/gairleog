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

  export void run(jute::view fname, jute::view src) {
    using namespace lispy;

    temp_frame ctx {};
    ctx.fns["sprdef"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 3) erred(n, "sprdef expects def name, id and frame count");
      map()[to_atom(aa[0])] = {
        .id  = lispy::to_u32(aa[1]),
        .qty = lispy::to_u32(aa[2]),
      };
      return n;
    };
    glispy::const_run(fname, src);
  }
}
