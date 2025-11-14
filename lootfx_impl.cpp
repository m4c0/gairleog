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
      if (as != 2) err(n, "expecting a name and an action");
      if (!is_atom(aa[0])) err(aa[0], "expecting an atom as the name");
      data.nodes[aa[0]->atom] = aa[1];
      data.keys.push_back_doubling(aa[0]->atom);
      return n;
    };
    ctx.run(data.src);
  } catch (const parser_error & e) {
    throw to_file_err("lootfx.lsp", e);
  }

  static action_list_t * current;
  template<action A>
  static const node * act(const node * n, const node * const * aa, unsigned as) {
    if (as != 0) err(n, "expecting no parameter");
    current->push_back(A);
    return n;
  }
  void apply(jute::view key, action_list_t * r) try {
    if (!data.nodes.has(key)) return;
    current = r;

    basic_context<node> ctx {};
    ctx.fns["damage"]   = act<action::damage>;
    ctx.fns["defence"]  = act<action::defence>;
    ctx.fns["heal"]     = act<action::heal>;
    ctx.fns["maxlife"]  = act<action::max_life>;
    ctx.fns["poison"]   = act<action::poison>;
    ctx.fns["strength"] = act<action::strength>;
    ctx.fns["weakness"] = act<action::weakness>;
    ctx.fns["wither"]   = act<action::wither>;
    ctx.eval(data.nodes[key]);
  } catch (const parser_error & e) {
    throw to_file_err("lootfx.lsp", e);
  }
}
