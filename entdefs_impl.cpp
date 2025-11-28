module entdefs;
import lispy;
import sprdef;

using namespace lispy;
using namespace lispy::experimental;

namespace entdefs {
  hashley::fin<t> defs { 127 };

  struct cnode : node, t {
    void (*attr)(cnode *, const cnode *);
  };
  struct context : basic_context<cnode> {
  };

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
  static jute::heap to_loot(const lispy::node * n) {
    if (!entdefs::has(n->atom)) lispy::err(n, "unknown entdef");
    return jute::heap { n->atom };
  }
  static unsigned to_spr(const lispy::node * name) {
    if (!lispy::is_atom(name)) lispy::err(name, "spr expects atom as name");
    if (!sprdef::has(name->atom)) lispy::err(name, "invalid sprite name");
    return sprdef::get(name->atom).id;
  }
  static auto to_spr_pair(const lispy::node * name) {
    if (!lispy::is_atom(name)) lispy::err(name, "spr expects atom as name");
    if (!sprdef::has(name->atom)) lispy::err(name, "invalid sprite name");
    return sprdef::get(name->atom);
  }

  template<flags Flags>
  static constexpr const auto mem_set = lispy::experimental::mem_set<&cnode::attr, [](auto * self, auto * n) {
    u32flags u {};
    u.u32 = bit_of(Flags) | bit_of(self->flags);
    self->flags = u.f;
  }>;

  static const auto entdef_ctx = [] {
    basic_context<cnode> ctx {};
    ctx.fns["atkspr"]   = mem_fn<&cnode::attr, &cnode::attack_sprite, to_spr_pair>;
    ctx.fns["defense"]  = mem_fn<&cnode::attr, &cnode::defense,       to_i>;
    ctx.fns["life"]     = mem_fn<&cnode::attr, &cnode::life,          to_life>;
    ctx.fns["light"]    = mem_fn<&cnode::attr, &cnode::light,         to_light>;
    ctx.fns["loot"]     = mem_fn<&cnode::attr, &cnode::loot,          to_loot>;
    ctx.fns["maxlife"]  = mem_fn<&cnode::attr, &cnode::max_life,      to_life>;
    ctx.fns["spr"]      = mem_fn<&cnode::attr, &cnode::sprite,        to_spr>;
    ctx.fns["strength"] = mem_fn<&cnode::attr, &cnode::strength,      to_i>;

    ctx.fns["ceramic"] = mem_set<{ .ceramic = true }>;
    ctx.fns["enemy"]   = mem_set<{ .enemy   = true }>;
    ctx.fns["exit"]    = mem_set<{ .exit    = true }>;
    ctx.fns["food"]    = mem_set<{ .food    = true }>;
    ctx.fns["player"]  = mem_set<{ .player  = true }>;
    ctx.fns["solid"]   = mem_set<{ .solid   = true }>;
    ctx.fns["toad"]    = mem_set<{ .toad    = true }>;
    return ctx;
  }();

  void run(jute::view src) try {
    context ctx {};
    ctx.fns["entdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as < 1) lispy::err(n, "entdef expects a name and attributes");
      if (!is_atom(aa[0])) lispy::err(aa[0], "expecting an atom as the entdef name");

      basic_context<cnode> ctx {};
      ctx.parent = &entdef_ctx;
      auto nn = fill_clone<cnode>(&ctx, n, aa + 1, as - 1);
      defs[aa[0]->atom] = *nn;
      return nn;
    };
    ctx.run(src);
  } catch (const lispy::parser_error & e) {
    throw lispy::to_file_err("entdefs.lsp", e);
  }

  bool has(jute::view name) {
    return defs.has(name);
  }
  const t & get(jute::view name) {
    return defs[name];
  }
}
