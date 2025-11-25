module lootfx;
import hai;
import hashley;
import lispy;
import silog;

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

  static hai::chain<hai::cstr> strbuf { 128 };
  static int lfxs_rem = ~0;
  bool read(file::reader * r, unsigned id, unsigned sz) {
    switch (id) {
      case 'LFXS': {
        r->read('LFXS', &lfxs_rem, sizeof(lfxs_rem));
        silog::assert(lfxs_rem == map.size(), "Invalid LFXS");
        return true;
      }
      case 'LFXM': {
        silog::assert(lfxs_rem > 0, "LFXM buffer underrun");
        silog::assert(lfxs_rem <= map.size(), "LFXM integer overflow or missing LFXS");

        auto idx = map.size() - lfxs_rem--;
        if (!sz) {
          map[idx] = {};
          r->read('LFXM', nullptr, 0);
          return true;
        }

        hai::cstr buf { sz };
        r->read('LFXM', buf.data(), buf.size());
        map[idx] = buf;

        strbuf.push_back(traits::move(buf));
        return true;
      }
      default: return false;
    }
  }
  void write(file::writer * w) {
    unsigned i = map.size();
    w->write('LFXS', &i, sizeof(i));
    for (auto str : map) w->write('LFXM', str.data(), str.size()); 
  }
}
