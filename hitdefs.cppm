export module hitdefs;
import jute;
import hai;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace hitdefs {
  hai::cstr g_source {};

  export void run(jute::view src) { g_source = src.cstr(); }
   
  const node * eval(const node * n) {
    struct context : basic_context<node> {
    } ctx {
      basic_context<node> { n->ctx->allocator }
    };
    ctx.fns["block"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 0) lispy::err(n, "block does not take arguments");
      return n;
    };
    ctx.fns["damage"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 1) lispy::err(n, "damage requires a single argument");
      auto _ = to_i(aa[0]);
      return n;
    };
    ctx.fns["miss"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 0) lispy::err(n, "miss does not take arguments");
      return n;
    };
    ctx.fns["pick"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 0) lispy::err(n, "pick does not take arguments");
      return n;
    };
    ctx.fns["poison"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 0) lispy::err(n, "poison does not take arguments");
      return n;
    };
    return ctx.eval(n);
  }

  export void check(jute::view from, jute::view to) {
    struct context : basic_context<node> {
      jute::view from;
      jute::view to;
    } ctx {
      .from = from,
      .to = to,
    };
    ctx.fns["hitdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 3) lispy::err(n, "hitdef requires source, target and action");
      if (!is_atom(aa[0])) lispy::err("source must be an atom");
      if (!is_atom(aa[1])) lispy::err("target must be an atom");

      auto ctx = static_cast<context *>(n->ctx);
      if (ctx->from != aa[0]->atom) return n;
      if (ctx->to   != aa[1]->atom) return n;
      return eval(aa[2]);
    };
    ctx.run(g_source);
  }
}
