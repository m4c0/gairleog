export module roomdefs;
import hai;
import hashley;
import jute;
import lispy;
import rng;
import sprdef;
import traits;

namespace roomdefs {
  export struct tiledef {
    bool block {};
    int light {};
    unsigned sprite {};
  };
  export struct t {
    unsigned w {};
    unsigned h {};
    hai::array<tiledef> d {};
    tiledef (*fn)(const t & t, unsigned x, unsigned y) {};
  };

  hai::cstr g_src {};
  export void run(jute::view src) { g_src = src.cstr(); }

  export hai::sptr<t> for_size(unsigned ew, unsigned eh) {
    struct node : lispy::node {
      enum { t_empty, t_block, t_light, t_spr, t_tdef, t_room } type {};
      tiledef tdef {};
      hai::sptr<t> room {};
    };
    struct context : lispy::context {
      unsigned w {};
      unsigned h {};
      const node * theme {};
    } ctx {
      { .allocator = lispy::allocator<node>() },
      ew, eh,
    }; 

    constexpr const auto eval = lispy::eval<node>;

    ctx.fns["block"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 0) lispy::err(n, "block does not accept parameters");
      return new (n->ctx->allocator()) node { *n, node::t_block, { .block = true } };
    };
    ctx.fns["light"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 1) lispy::err(n, "light requires intensity as parameter");
      auto i = lispy::to_i(eval(n->ctx, aa[0]));
      if (i < 0 || i > 15) lispy::err(n, "light intensity should be between 0 and 15");
      return new (n->ctx->allocator()) node { *n, node::t_light, { .light = i } };
    };
    ctx.fns["spr"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 1) lispy::err(n, "spr requires only the sprite name");

      if (!lispy::is_atom(aa[0])) lispy::err(aa[0], "spr expects atom as name");
      if (!sprdef::has(aa[0]->atom)) lispy::err(aa[0], "invalid sprite name");

      auto id = sprdef::get(aa[0]->atom);
      return new (n->ctx->allocator()) node { *n, node::t_spr, { .sprite = id } };
    };
    ctx.fns["tile"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      tiledef t {};
      for (auto i = 0; i < as; i++) {
        auto a = eval(n->ctx, aa[i]);
        switch (a->type) {
          default:
            lispy::err(aa[i], "expecting spr, light or block inside a tdef");
            break;
          case node::t_block:
            t.block = a->tdef.block;
            break;
          case node::t_light:
            t.light = a->tdef.light;
            break;
          case node::t_spr:
            t.sprite = a->tdef.sprite;
            break;
        }
      }
      return new (n->ctx->allocator()) node { *n, node::t_tdef, { t } };
    };

    ctx.fns["themedef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 1) lispy::err(n, "themedef requires at exactly one parameter");
      static_cast<context *>(n->ctx)->theme = static_cast<const node *>(aa[0]);
      return aa[0];
    };
    ctx.fns["room"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as < 2) lispy::err(n, "rooms must have at least two rows");

      auto ctx = static_cast<context *>(n->ctx);

      unsigned cols = aa[0]->atom.size();

      if (!(as == ctx->h && cols == ctx->w) &&
          !(as == ctx->w && cols == ctx->h)) return {};

      if (!ctx->theme) lispy::err(n, "must define theme before rooms");
      auto _ = lispy::eval<node>(ctx, ctx->theme);

      hai::array<tiledef> data { as * cols };
      for (auto i = 0; i < as; i++) {
        if (!lispy::is_atom(aa[i])) lispy::err(aa[i], "all rows must be atoms");
        auto a = aa[i]->atom;
        if (cols != a.size()) lispy::err(aa[i], "all rows must have the same length");
        for (auto idx = 0; idx < cols; idx++) {
          auto c = a.subview(idx, 1).middle;
          if (!n->ctx->defs.has(c)) lispy::err(aa[i], "unknown def", idx);

          auto cell = lispy::eval<node>(n->ctx, n->ctx->defs[c]);
          if (!lispy::is_atom(cell)) lispy::err(aa[i], "cell must be a sprite name", idx);

          if (!ctx->defs.has(cell->atom)) lispy::err(cell, "unknown tiledef");
          auto tdn = lispy::eval<node>(ctx, ctx->defs[cell->atom]);
          if (tdn->type != node::t_tdef) lispy::err(tdn, "expecting a tiledef");

          data[i * cols + idx] = tdn->tdef;
        }
      }

      hai::sptr r { new t {
        .w = cols,
        .h = as,
        .d = traits::move(data),
      }}; 
      if (ctx->w == r->w) {
        r->fn = [](auto t, auto x, auto y) {
          return t.d[y * t.w + x];
        };
      } else {
        r->fn = [](auto t, auto x, auto y) {
          return t.d[x * t.w + y];
        };
      }
      return new (n->ctx->allocator()) node { *n, node::t_room, {}, r };
    };
    ctx.fns["roomdefs"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as == 0) lispy::err(n, "missing at least one room option");

      hai::varray<const node *> opts { as };
      for (auto i = 0; i < as; i++) {
        auto a = lispy::eval<node>(n->ctx, aa[i]);
        if (a) opts.push_back(a);
      }
      if (opts.size() == 0) return {};
      return opts[rng::rand(opts.size())];
    };
    
    auto n = lispy::run<node>(g_src, &ctx);
    if (!n || n->type != node::t_room) return {};
    
    return n && n->type == node::t_room ? n->room : hai::sptr<t> {};
  }
}
