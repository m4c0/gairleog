module lootfx;
import glispy;
import hai;
import hashley;
import silog;

using namespace lispy;
using namespace lispy::experimental;

namespace lootfx {
  struct {
    jute::heap src {};
    hashley::fin<const node *> nodes { 127 };
    hai::varray<jute::view> keys { 128 };
    hai::uptr<arena<node>> arena {};
  } data {};

  void reset() {
    for (auto & m : map) m = {};

    rest.truncate(0);
    for (auto k : data.keys) rest.push_back_doubling(jute::heap { k });
  }

  void run(jute::view src) try {
    data = {
      .src = jute::heap { src },
      .arena = arena<node>::make(),
    };
    auto a = data.arena->use();

    glispy::temp_frame ctx {};
    ctx.fns["fx"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 2) erred(n, "expecting a name and an action");
      if (!is_atom(aa[0])) erred(aa[0], "expecting an atom as the name");
      data.nodes[aa[0]->atom] = aa[1];
      data.keys.push_back_doubling(aa[0]->atom);
      return n;
    };
    lispy::run<node>(data.src);
  } catch (const parser_error & e) {
    throw to_file_err("lootfx.lsp", e);
  }

  static action_list_t * current;
  template<action A>
  static const node * act(const node * n, const node * const * aa, unsigned as) {
    if (as != 0) erred(n, "expecting no parameter");
    current->push_back(A);
    return n;
  }
  void apply(jute::view key, action_list_t * r) try {
    if (!data.nodes.has(key)) return;
    current = r;

    glispy::temp_frame ctx {};
    ctx.fns["damage"]   = act<action::damage>;
    ctx.fns["defence"]  = act<action::defence>;
    ctx.fns["heal"]     = act<action::heal>;
    ctx.fns["maxlife"]  = act<action::max_life>;
    ctx.fns["poison"]   = act<action::poison>;
    ctx.fns["strength"] = act<action::strength>;
    ctx.fns["weakness"] = act<action::weakness>;
    ctx.fns["wither"]   = act<action::wither>;
    auto _ = eval<node>(data.nodes[key]);
  } catch (const parser_error & e) {
    throw to_file_err("lootfx.lsp", e);
  }

  void read(file::reader * r) {
    for (auto & v : map) v = r->read<jute::heap>();

    rest.truncate(0);
    auto len = r->read<unsigned>();
    for (auto i = 0; i < len; i++) rest.push_back_doubling(r->read<jute::heap>());
  }
  void write(file::writer * w) {
    for (auto v : map) w->write(v);

    w->write<unsigned>(rest.size());
    for (auto v : rest) w->write(v);
  }
}
