export module entdefs;
import jute;
import hashley;
import lispy;
import sprdef;
import tiledefs;

using namespace lispy;
using namespace lispy::experimental;

namespace entdefs {
  export struct error : lispy::parser_error {};

  hashley::fin<tiledefs::t> defs { 127 };

  export bool has(jute::view name) {
    return defs.has(name);
  }
  export const auto & get(jute::view name) {
    return defs[name];
  }

  static float to_light(const lispy::node * n) {
    auto i = lispy::to_f(n);
    if (i < 0 || i > 1) lispy::err(n, "light intensity should be between 0 and 1");
    return i;
  }
  static float to_life(const lispy::node * n) {
    auto i = lispy::to_i(n);
    if (i < 1) lispy::err(n, "life should be greater than 0");
    return i;
  }
  static unsigned to_spr(const lispy::node * name) {
    if (!lispy::is_atom(name)) lispy::err(name, "spr expects atom as name");
    if (!sprdef::has(name->atom)) lispy::err(name, "invalid sprite name");
    return sprdef::get(name->atom);
  }

  struct cnode : node, tiledefs::t {
    void (*attr)(cnode *, const cnode *);
  };
  struct context : basic_context<cnode> {
  };
  export void run(jute::view src) try {
    context ctx {};
    ctx.fns["entdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as < 1) lispy::err(n, "entdef expects a name and attributes");
      if (!is_atom(aa[0])) lispy::err(aa[0], "expecting an atom as the entdef name");

      context ctx { basic_context<cnode>{ n->ctx->allocator } };
      ctx.fns["light"] = mem_fn<&cnode::attr, &cnode::light,  to_light>;
      ctx.fns["life"]  = mem_fn<&cnode::attr, &cnode::life,   to_life>;
      ctx.fns["spr"]   = mem_fn<&cnode::attr, &cnode::sprite, to_spr>;
      tiledefs::lispy<cnode>(ctx);
      auto nn = fill_clone<cnode>(&ctx, n, aa + 1, as - 1);
      defs[aa[0]->atom] = *nn;
      return nn;
    };
    ctx.run(src);
  } catch (const lispy::parser_error & e) {
    throw error { e };
  }
}
