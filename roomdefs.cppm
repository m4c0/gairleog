#pragma leco add_resource roomdefs.lsp
export module roomdefs;
import hai;
import jute;
import lispy;
import rng;
import sires;
import tiledefs;
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

  void run(jute::view src) {
    struct node : lispy::node {
      hai::sptr<t> room {};
    };
    struct context : lispy::context {
      const node * theme;
    } ctx {
      { .allocator = lispy::allocator<node>() },
    }; 

    //constexpr const auto eval = lispy::eval<node>;

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
      auto _ = eval<node>(ctx, ctx->theme);

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

          if (!tiledefs::has(cell->atom)) lispy::err(cell, "unknown sprdef");
          auto & spr = tiledefs::get(cell->atom).sprite;
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

  export void load(hai::fn<void> cb) {
    sires::read("roomdefs.lsp", nullptr, [cb](auto ptr, hai::cstr & src) mutable {
      run(src);
      cb();
    });
  }
}
