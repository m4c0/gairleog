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

  export struct t {
    unsigned w {};
    unsigned h {};
    hai::array<unsigned> data {};
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
    struct tiledef {
      bool block;
      int light {};
      hai::array<unsigned> sprite {};
    };

    struct node : lispy::node {
      enum { t_empty, t_block, t_light, t_spr } type {};
      tiledef tdef {};
      hai::sptr<t> room {};
    };
    struct context : lispy::context {
      const node * theme;
      hashley::fin<tiledef> tdefs { 127 };
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
      if (as == 0) lispy::err(n, "spr requires at least one name");

      auto nn = new (n->ctx->allocator()) node { *n, node::t_spr, { .sprite { as } } };
      for (auto i = 0; i < as; i++) {
        if (!lispy::is_atom(aa[i])) lispy::err(aa[i], "spr expects atom names");
        if (!sprdef::has(aa[i]->atom)) lispy::err(aa[i], "invalid sprite name");
        nn->tdef.sprite[i] = sprdef::get(aa[i]->atom);
      }
      return nn;
    };
    ctx.fns["tiledef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as < 2) lispy::err(n, "tiledef requires at least name and spr");
      if (!lispy::is_atom(aa[0])) lispy::err(n, "tiledef name should be an atom");

      auto & t = static_cast<context *>(n->ctx)->tdefs[aa[0]->atom];
      for (auto i = 1; i < as; i++) {
        auto a = eval(n->ctx, aa[i]);
        switch (a->type) {
          case node::t_empty:
            lispy::err(aa[i], "expecting spr, light or block inside a tdef");
            break;
          case node::t_block:
            t.block = a->tdef.block;
            break;
          case node::t_light:
            t.light = a->tdef.light;
            break;
          case node::t_spr:
            t.sprite.set_capacity(a->tdef.sprite.size());
            for (auto i = 0; i < t.sprite.size(); i++) {
              t.sprite[i] = a->tdef.sprite[i];
            }
            break;
        }
      }

      return n;
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
      hai::array<unsigned> data { cols * as };
      for (auto i = 0; i < as; i++) {
        if (!lispy::is_atom(aa[i])) lispy::err(aa[i], "all rows must be atoms");
        auto a = aa[i]->atom;
        if (cols != a.size()) lispy::err(aa[i], "all rows must have the same length");
        for (auto idx = 0; idx < a.size(); idx++) {
          auto c = a.subview(idx, 1).middle;
          if (!n->ctx->defs.has(c)) lispy::err(aa[i], "unknown def", idx);

          auto cell = lispy::eval<node>(n->ctx, n->ctx->defs[c]);
          if (!lispy::is_atom(cell)) lispy::err(aa[i], "cell must be a sprite name", idx);

          if (!ctx->tdefs.has(cell->atom)) lispy::err(cell, "unknown sprdef");
          auto & spr = ctx->tdefs[cell->atom].sprite;
          data[i * cols + idx] = spr[rng::rand(spr.size())];
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
