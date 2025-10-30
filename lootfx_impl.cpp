module lootfx;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace lootfx {
  void run(jute::view src) try {
    basic_context<node> ctx {};
    ctx.fns["fx"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 2) err("expecting a name and an action");
      if (!is_atom(aa[0])) err("expecting an atom as the name");

      basic_context<node> ctx { n->ctx->allocator };
      ctx.fns["heal"] = [](auto n, auto aa, auto as) -> const node * {
        return n;
      };
      ctx.fns["str"] = [](auto n, auto aa, auto as) -> const node * {
        return n;
      };
      ctx.eval(aa[1]);

      auto nn = clone<node>(n);
      return nn;
    };

    ctx.run(src);
  } catch (const parser_error & e) {
    throw to_file_err("lootfx.lsp", e);
  }
}
