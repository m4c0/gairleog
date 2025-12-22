module entdefs;
import glispy;
import hai;
import hashley;
import sfxdefs;
import sprdef;

using namespace lispy;
using namespace lispy::experimental;

namespace entdefs {
  struct entdef {
    const node * n;
    hai::array<const node *> args;
  };
  hashley::fin<entdef> defs { 127 };

  static float to_light(const lispy::node * n) {
    auto i = lispy::to_f(n);
    if (i < 0 || i > 1) lispy::erred(n, "light intensity should be between 0 and 1");
    return i;
  }
  static float to_life(const lispy::node * n) {
    auto i = lispy::to_i(n);
    if (i < 1) lispy::erred(n, "life should be greater than 0");
    return i;
  }
  static jute::heap to_loot(const lispy::node * n) {
    if (!entdefs::has(n->atom)) lispy::erred(n, "unknown entdef");
    return jute::heap { n->atom };
  }
  static unsigned to_spr(const lispy::node * name) {
    if (!lispy::is_atom(name)) lispy::erred(name, "spr expects atom as name");
    if (!sprdef::has(name->atom)) lispy::erred(name, "invalid sprite name");
    return sprdef::get(name->atom).id;
  }
  static auto to_spr_pair(const lispy::node * name) {
    if (!lispy::is_atom(name)) lispy::erred(name, "spr expects atom as name");
    if (!sprdef::has(name->atom)) lispy::erred(name, "invalid sprite name");
    return sprdef::get(name->atom);
  }

  template<auto Attr, auto Fn>
  static constexpr const node * conv(const node * n, const node * const * aa, unsigned as) {
    if (as != 1) erred(n, "Expecting a single parameter");
    auto * obj = static_cast<t *>(context()->ptr("t"));
    obj->*Attr = Fn(eval<node>(aa[0]));
    return n;
  }

  template<flags Flags>
  static constexpr const node * set(const node * n, const node * const * aa, unsigned as) {
    if (as != 0) erred(n, "Expecting no parameters");
    auto * obj = static_cast<t *>(context()->ptr("t"));

    u32flags u {};
    u.u32 = bit_of(Flags) | bit_of(obj->flags);
    obj->flags = u.f;

    return n;
  }

  static auto entdef_ctx = [] {
    auto ctx = lispy::frame::make();
    ctx->fns["atkspr"]   = conv<&t::attack_sprite, to_spr_pair>;
    ctx->fns["defense"]  = conv<&t::defense,       to_i>;
    ctx->fns["life"]     = conv<&t::max_life,      to_life>;
    ctx->fns["light"]    = conv<&t::light,         to_light>;
    ctx->fns["loot"]     = conv<&t::loot,          to_loot>;
    ctx->fns["rotation"] = conv<&t::rotation,      to_i>;
    ctx->fns["splatspr"] = conv<&t::splat_sprite,  to_spr_pair>;
    ctx->fns["spr"]      = conv<&t::sprite,        to_spr>;
    ctx->fns["strength"] = conv<&t::strength,      to_i>;

    ctx->fns["ceramic"]     = set<{ .ceramic     = true }>;
    ctx->fns["enemy"]       = set<{ .enemy       = true }>;
    ctx->fns["exit"]        = set<{ .exit        = true }>;
    ctx->fns["fluorescent"] = set<{ .fluorescent = true }>;
    ctx->fns["food"]        = set<{ .food        = true }>;
    ctx->fns["player"]      = set<{ .player      = true }>;
    ctx->fns["poison"]      = set<{ .poison      = true }>;
    ctx->fns["rotates"]     = set<{ .rotates     = true }>;
    ctx->fns["solid"]       = set<{ .solid       = true }>;
    ctx->fns["wall"]        = set<{ .wall        = true }>;
    return ctx;
  }();

  static auto src_ctx = [] {
    auto ctx = lispy::frame::make();
    ctx->fns["entdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as < 1) lispy::erred(n, "entdef expects a name and attributes");
      if (!is_atom(aa[0])) lispy::erred(aa[0], "expecting an atom as the entdef name");

      auto & d = defs[aa[0]->atom] = {
        .n = n,
        .args = hai::array<const node *>{ as - 1 },
      };
      for (auto i = 0; i < as - 1; i++) d.args[i] = aa[i + 1];
      return n;
    };
    return ctx;
  }();

  void run(jute::view src) {
    static auto src_arena = arena<node>::make();
    auto a = src_arena->use();
    frame_guard c { src_ctx };
    lispy::run<node>("entdefs.lsp", src);
  }

  bool has(jute::view name) {
    return defs.has(name);
  }
  t get(jute::view name) {
    auto & d = defs[name];

    frame_guard c1 { src_ctx };
    frame_guard c2 { entdef_ctx };
    temp_frame c {};

    t res {
      .name { name },
    };
    c.ptrs["t"] = &res;

    for (const node * arg : d.args) auto _ = eval<node>(arg);
    return res;
  }
}
