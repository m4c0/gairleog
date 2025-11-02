module lootfx;
import hai;
import hashley;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace lootfx {
  struct {
    jute::heap src {};
    hai::fn<node *> alloc {};
    hashley::fin<const node *> nodes { 127 };
    hai::varray<jute::view> keys { 128 };
  } data {};

  void reset() {
    for (auto & m : map) m = {};

    rest.truncate(0);
    for (auto k : data.keys) rest.push_back_doubling(k);
  }

  void run(jute::view src) try {
    data = {};
    data.alloc = allocator<node>();
    data.src = jute::heap { src };

    basic_context<node> ctx { data.alloc };
    ctx.fns["fx"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 2) err("expecting a name and an action");
      if (!is_atom(aa[0])) err("expecting an atom as the name");
      data.nodes[aa[0]->atom] = aa[1];
      data.keys.push_back_doubling(aa[0]->atom);
      return n;
    };
    ctx.run(data.src);
  } catch (const parser_error & e) {
    throw to_file_err("lootfx.lsp", e);
  }

  void apply(jute::view key) try {
    if (!data.nodes.has(key)) return;

    basic_context<node> ctx {};
    ctx.fns["heal"] = [](auto n, auto aa, auto as) -> const node * {
      return n;
    };
    ctx.fns["str"] = [](auto n, auto aa, auto as) -> const node * {
      return n;
    };
    ctx.eval(data.nodes[key]);
  } catch (const parser_error & e) {
    throw to_file_err("lootfx.lsp", e);
  }
}
