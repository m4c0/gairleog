export module hitdefs;
import entdefs;
import jute;
import hai;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace hitdefs {
  export enum class action {
    block,
    exit,
    hit,
    miss,
    pick,
    poison,
  };
  export using action_list_t = hai::chain<action>;

  struct context : basic_context<node> {
    action_list_t * result;
  };

  hai::cstr g_source {};

  export void run(jute::view src) { g_source = src.cstr(); }

  template<action Fn>
  static inline constexpr const auto ref = [](auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 0) lispy::erred(n, "actions do not take arguments");
    static_cast<context *>(n->ctx)->result->push_back(Fn);
    return n;
  };

  const node * eval(const node * n, action_list_t * result) {
    context ctx { {}, result };
    ctx.fns["block"]  = ref<action::block>;
    ctx.fns["exit"]   = ref<action::exit>;
    ctx.fns["hit"]    = ref<action::hit>;
    ctx.fns["miss"]   = ref<action::miss>;
    ctx.fns["pick"]   = ref<action::pick>;
    ctx.fns["poison"] = ref<action::poison>;
    return ctx.eval(n);
  }

  export action_list_t check(entdefs::flags from, entdefs::flags to) try {
    lispy::temp_arena<node> a {};

    action_list_t result { 8 };
    struct context : hitdefs::context {
      unsigned from;
      unsigned to;
    } ctx {};

    ctx.from = entdefs::bit_of(from);
    ctx.to   = entdefs::bit_of(to);
    ctx.result = &result;
    ctx.fns["hitdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 3) lispy::erred(n, "hitdef requires source, target and action");
      if (!is_atom(aa[0])) lispy::erred(aa[0], "source must be an atom");
      if (!is_atom(aa[1])) lispy::erred(aa[1], "target must be an atom");

      auto from = entdefs::bit_of(aa[0]->atom);
      auto to   = entdefs::bit_of(aa[1]->atom);

      if (!from) lispy::erred(aa[0], "unknown component");
      if (!to)   lispy::erred(aa[1], "unknown component");

      auto ctx = static_cast<context *>(n->ctx);
      if ((ctx->from & from) && (ctx->to & to)) eval(aa[2], ctx->result);
      return n;
    };
    ctx.run(g_source);
    return result;
  } catch (const lispy::parser_error & e) {
    throw lispy::to_file_err("hitdefs.lsp", e);
  }
}
