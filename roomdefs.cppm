export module roomdefs;
import hai;
import hashley;
import jute;
import lispy;
import rng;
import sprdef;
import traits;

namespace roomdefs {
  export constexpr const auto max_size = 8;

  export struct tiledef {
    bool block {};
    int light {};
    unsigned sprite {};
  };
  export struct t {
    unsigned w {};
    unsigned h {};
    hai::array<tiledef> data {};
  };
  auto & list() {
    static hai::varray<hai::sptr<t>> i[max_size][max_size] {};
    return i;
  }

  export hai::sptr<t> for_size(unsigned w, unsigned h) {
    auto & r = list()[h][w];
    if (r.size() == 0) return {};
    return r[rng::rand(r.size())];
  }

  export void run(jute::view src) {
    struct node : lispy::node {
      enum { t_empty, t_block, t_light, t_spr, t_tdef } type {};
      tiledef tdef {};
      hai::sptr<t> room {};
    };
    struct context : lispy::context {
      const node * theme;
    } ctx {
      { .allocator = lispy::allocator<node>() },
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
          case node::t_empty:
          case node::t_tdef:
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
      if (as > max_size) lispy::err(n, "rooms is too long");

      auto ctx = static_cast<context *>(n->ctx);
      if (!ctx->theme) lispy::err(n, "must define theme beforehand");
      auto _ = lispy::eval<node>(ctx, ctx->theme);

      unsigned cols = aa[0]->atom.size();
      if (cols > max_size) lispy::err(aa[0], "row is too wide");
      hai::array<tiledef> data { cols * as };
      for (auto i = 0; i < as; i++) {
        if (!lispy::is_atom(aa[i])) lispy::err(aa[i], "all rows must be atoms");
        auto a = aa[i]->atom;
        if (cols != a.size()) lispy::err(aa[i], "all rows must have the same length");
        for (auto idx = 0; idx < a.size(); idx++) {
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

      list()[as - 1][cols - 1].push_back_doubling(hai::sptr { new t {
        .w = cols,
        .h = as,
        .data = traits::move(data),
      }}); 

      return n;
    };
    
    lispy::run(src, &ctx);
  }
}
