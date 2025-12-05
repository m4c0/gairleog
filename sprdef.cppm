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

  struct custom_node : lispy::node {
    int spr_id;
    bool valid;
  };
  static auto sprdef(const lispy::node * n, const custom_node * def, const custom_node * val, const custom_node * qty) {
    if (!lispy::is_atom(def)) lispy::erred(def, "def name must be an atom");
    map()[def->atom] = {
      .id  = lispy::to_u32(val),
      .qty = lispy::to_u32(qty),
    };
    return n;
  }
  // TODO: remove try/catch
  export void run(jute::view fname, jute::view src) {
    lispy::temp_arena<custom_node> a {};
    lispy::temp_frame ctx {};
    ctx.fns["sprdef"] = lispy::experimental::wrap<custom_node, sprdef>;
    lispy::run<custom_node>(fname, src);
  }
}
