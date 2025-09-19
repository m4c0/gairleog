export module sprdef;
import hai;
import hashley;
import jute;
import lispy;
import sires;
import traits;

namespace sprdef {
  auto & map() {
    static hashley::niamh i { 1023 };
    return i;
  }
  export bool has(jute::view key) { return map().has(key); }
  export const auto & get(jute::view key) { return map()[key]; }

  export void run(jute::view src) {
    struct custom_node : lispy::node {
      int spr_id;
      bool valid;
    };
    lispy::context ctx {
      .allocator = lispy::allocator<custom_node>(),
    };

    ctx.fns["sprdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 2) lispy::err(n, "def expects name and value");
      if (!lispy::is_atom(aa[0])) lispy::err(aa[0], "def name must be an atom");
      
      map()[aa[0]->atom] = lispy::to_i(eval<custom_node>(n->ctx, aa[1]));

      return n;
    };
    lispy::run<custom_node>(src, &ctx);
  }
}
