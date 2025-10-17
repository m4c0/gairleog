export module entdefs;
import jute;
import lispy;
import tiledefs;

using namespace lispy;
using namespace lispy::experimental;

namespace entdefs {
  export struct error : lispy::parser_error {};

  struct cnode : node, tiledefs::t {
    void (*attr)(cnode *, const cnode *);
  };
  struct context : basic_context<cnode> {
  };
  export void run(jute::view src) {
    context ctx {};
    ctx.fns["entdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      context ctx { basic_context<cnode>{ n->ctx->allocator } };
      tiledefs::lispy<cnode>(ctx);
      return n;
    };
    ctx.run(src);
  }
}
