export module hitdefs;
import jute;
import hai;
import lispy;

import print;

using namespace lispy;
using namespace lispy::experimental;

namespace hitdefs::fns {
  void block() { putln("block"); }
  void damage(int n) { putln("damage ", n); }
  void miss() { putln("miss"); }
  void pick() { putln("pick"); }
  void poison() { putln("poison"); }
}

namespace hitdefs {
  export using action_t = hai::fn<void>;
  export using action_list_t = hai::chain<action_t>;

  struct context : basic_context<node> {
    action_list_t * result;
    jute::view from;
    jute::view to;
  };

  hai::cstr g_source {};

  export void run(jute::view src) { g_source = src.cstr(); }

  const node * eval(const node * n, action_list_t * result) {
    context ctx { basic_context<node> { n->ctx->allocator }, result };
    ctx.fns["block"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 0) lispy::err(n, "block does not take arguments");
      static_cast<context *>(n->ctx)->result->push_back(fns::block);
      return n;
    };
    ctx.fns["damage"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 1) lispy::err(n, "damage requires a single argument");
      auto i = to_i(aa[0]);
      static_cast<context *>(n->ctx)->result->push_back([i] { fns::damage(i); });
      return n;
    };
    ctx.fns["miss"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 0) lispy::err(n, "miss does not take arguments");
      static_cast<context *>(n->ctx)->result->push_back(fns::miss);
      return n;
    };
    ctx.fns["pick"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 0) lispy::err(n, "pick does not take arguments");
      static_cast<context *>(n->ctx)->result->push_back(fns::pick);
      return n;
    };
    ctx.fns["poison"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 0) lispy::err(n, "poison does not take arguments");
      static_cast<context *>(n->ctx)->result->push_back(fns::poison);
      return n;
    };
    return ctx.eval(n);
  }

  export action_list_t check(jute::view from, jute::view to) {
    action_list_t result { 8 };
    context ctx {
      .result = &result,
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
      return eval(aa[2], ctx->result);
    };
    ctx.run(g_source);
    return result;
  }
}
