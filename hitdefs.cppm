export module hitdefs;
import jute;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace hitdefs {
  export void run(jute::view src) {
    basic_context<node> ctx {};
    ctx.fns["hitdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 3) lispy::err(n, "hitdef requires source, target and action");

      auto nn = clone<node>(n);
      return nn;
    };
    ctx.run(src);
  }
}
