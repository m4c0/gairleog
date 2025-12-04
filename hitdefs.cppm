export module hitdefs;
import entdefs;
import glispy;
import hai;
import jute;

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

  hai::cstr g_source {};

  export void run(jute::view src) { g_source = src.cstr(); }

  template<action Fn>
  static inline constexpr const auto ref = [](auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 0) lispy::erred(n, "actions do not take arguments");
    static_cast<action_list_t *>(lispy::context()->ptr("result"))->push_back(Fn);
    return n;
  };

  const node * eval(const node * n) {
    lispy::temp_frame ctx {};
    ctx.fns["block"]  = ref<action::block>;
    ctx.fns["exit"]   = ref<action::exit>;
    ctx.fns["hit"]    = ref<action::hit>;
    ctx.fns["miss"]   = ref<action::miss>;
    ctx.fns["pick"]   = ref<action::pick>;
    ctx.fns["poison"] = ref<action::poison>;
    return eval<node>(n);
  }

  export action_list_t check(entdefs::flags from, entdefs::flags to) try {
    lispy::temp_arena<node> a {};

    action_list_t result { 8 };

    lispy::temp_frame ctx {};
    ctx.ptrs["from"]   = &from;
    ctx.ptrs["to"]     = &to;
    ctx.ptrs["result"] = &result;

    ctx.fns["hitdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 3) lispy::erred(n, "hitdef requires source, target and action");
      if (!is_atom(aa[0])) lispy::erred(aa[0], "source must be an atom");
      if (!is_atom(aa[1])) lispy::erred(aa[1], "target must be an atom");

      auto from = entdefs::bit_of(aa[0]->atom);
      auto to   = entdefs::bit_of(aa[1]->atom);

      if (!from) lispy::erred(aa[0], "unknown component");
      if (!to)   lispy::erred(aa[1], "unknown component");

      auto efrom = *static_cast<entdefs::flags *>(context()->ptr("from"));
      auto eto   = *static_cast<entdefs::flags *>(context()->ptr("to"));
      if ((entdefs::bit_of(efrom) & from) && (entdefs::bit_of(eto) & to)) eval(aa[2]);
      return n;
    };
    lispy::run<node>(g_source);
    return result;
  } catch (const lispy::parser_error & e) {
    throw lispy::to_file_err("hitdefs.lsp", e);
  }
}
