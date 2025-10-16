export module entdefs;
import jute;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace entdefs {
  export struct error : lispy::parser_error {};

  struct context : basic_context<node> {
  };
  export void run(jute::view src) {
    context ctx {};
    ctx.fns["entdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      return n;
    };
    ctx.run(src);
  }
}
