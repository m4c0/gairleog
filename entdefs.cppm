export module entdefs;
import jute;
import lispy;
import sprdef;
import tiledefs;

using namespace lispy;
using namespace lispy::experimental;

namespace entdefs {
  export struct error : lispy::parser_error {};

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
    int sprite;
  };
  struct context : basic_context<cnode> {
  };
  export void run(jute::view src) {
    context ctx {};
    ctx.fns["entdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      context ctx { basic_context<cnode>{ n->ctx->allocator } };
      ctx.fns["light"] = mem_fn<&cnode::attr, &cnode::light,  to_light>;
      ctx.fns["life"]  = mem_fn<&cnode::attr, &cnode::life,   to_life>;
      ctx.fns["spr"]   = mem_fn<&cnode::attr, &cnode::sprite, to_spr>;
      tiledefs::lispy<cnode>(ctx);
      return n;
    };
    ctx.run(src);
  }
}
