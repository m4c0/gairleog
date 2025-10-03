export module hitdefs;
import jute;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace hitdefs {
  export void run(jute::view src) {
    basic_context<node> ctx {};
    ctx.fns["hitdef"] = [](auto n, auto aa, auto an) {
      return n;
    };
    ctx.run(src);
  }
}
