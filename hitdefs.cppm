export module hitdefs;
import jute;
import hai;
import lispy;
import tiledefs;

import print;

using namespace lispy;
using namespace lispy::experimental;

namespace hitdefs::fns {
  void block() { putln("block"); }
  void hit() { putln("hit"); }
  void miss() { putln("miss"); }
  void pick() { putln("pick"); }
  void poison() { putln("poison"); }
}

namespace hitdefs {
  export struct error : lispy::parser_error {};

  export using action_t = void (*)(void);
  export using action_list_t = hai::chain<action_t>;

  struct context : basic_context<node> {
    action_list_t * result;
  };

  hai::cstr g_source {};

  export void run(jute::view src) { g_source = src.cstr(); }

  template<action_t Fn>
  static inline constexpr const auto ref = [](auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 0) lispy::err(n, "actions do not take arguments");
    static_cast<context *>(n->ctx)->result->push_back(Fn);
    return n;
  };

  const node * eval(const node * n, action_list_t * result) {
    context ctx { basic_context<node> { n->ctx->allocator }, result };
    ctx.fns["block"]  = ref<fns::block>;
    ctx.fns["hit"]    = ref<fns::hit>;
    ctx.fns["miss"]   = ref<fns::miss>;
    ctx.fns["pick"]   = ref<fns::pick>;
    ctx.fns["poison"] = ref<fns::poison>;
    return ctx.eval(n);
  }

  export action_list_t check(tiledefs::flags from, tiledefs::flags to) try {
    action_list_t result { 8 };
    struct context : hitdefs::context {
      unsigned from;
      unsigned to;
    } ctx {};

    ctx.from = tiledefs::bit_of(from);
    ctx.to   = tiledefs::bit_of(to);
    ctx.result = &result;
    ctx.fns["hitdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 3) lispy::err(n, "hitdef requires source, target and action");
      if (!is_atom(aa[0])) lispy::err(aa[0], "source must be an atom");
      if (!is_atom(aa[1])) lispy::err(aa[1], "target must be an atom");

      auto from = tiledefs::bit_of(aa[0]->atom);
      auto to   = tiledefs::bit_of(aa[1]->atom);

      if (!from) lispy::err(aa[0], "unknown component");
      if (!to)   lispy::err(aa[1], "unknown component");

      auto ctx = static_cast<context *>(n->ctx);
      if ((ctx->from & from) && (ctx->to & to)) eval(aa[2], ctx->result);
      return n;
    };
    ctx.run(g_source);
    return result;
  } catch (const lispy::parser_error & e) {
    throw error { e };
  }
}
