export module hitdefs;
import jute;
import hai;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace hitdefs {
  struct context : basic_context<node> {
    jute::view from;
    jute::view to;
  };

  hai::cstr g_source {};

  export void run(jute::view src) { g_source = src.cstr(); }
   
  export void check(jute::view from, jute::view to) {
    context ctx {
      .from = from,
      .to = to,
    };
    ctx.fns["hitdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 3) lispy::err(n, "hitdef requires source, target and action");
      if (!lispy::is_atom(aa[0])) lispy::err("source must be an atom");
      if (!lispy::is_atom(aa[1])) lispy::err("target must be an atom");

      auto ctx = static_cast<context *>(n->ctx);
      if (ctx->from != aa[0]->atom) return n;
      if (ctx->to   != aa[1]->atom) return n;

      auto nn = clone<node>(n);
      return nn;
    };
    ctx.run(g_source);
  }
}
