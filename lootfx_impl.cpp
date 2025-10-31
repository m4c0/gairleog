module lootfx;
import hai;
import hashley;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace lootfx {
  struct {
    hai::fn<node *> alloc {};
    hashley::fin<const node *> nodes { 127 };
  } data {};

  void run(jute::view src) try {
    data = {};
    data.alloc = allocator<node>();

    basic_context<node> ctx { data.alloc };
    ctx.fns["fx"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 2) err("expecting a name and an action");
      if (!is_atom(aa[0])) err("expecting an atom as the name");
      data.nodes[aa[0]->atom] = aa[1];
      return n;
    };
    ctx.run(src);
  } catch (const parser_error & e) {
    throw to_file_err("lootfx.lsp", e);
  }
}
