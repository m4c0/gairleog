#pragma leco add_resource roomdefs.lsp
export module roomdefs;
import hai;
import jojo;
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
  export struct list {
    hai::varray<hai::sptr<t>> data[max_size][max_size] {};
  };;

  [[nodiscard]] auto run(const tiledefs::map & tiledefs, jute::view src) {
    list rooms {};

    struct node : lispy::node {
      hai::sptr<t> room {};
    };
    struct context : lispy::context {
      const tiledefs::map * tiledefs;
      list * list;
    } ctx {
      { .allocator = lispy::allocator<node>() },
      &tiledefs,
      &rooms,
    }; 

    constexpr const auto eval = lispy::eval<node>;

    ctx.fns["random"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as == 0) err(n, "random requires at least a parameter");
      return eval(n->ctx, aa[rng::rand(as)]);
    };
    ctx.fns["room"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as < 2) lispy::err(n, "rooms must have at least two rows");
      if (as > max_size) lispy::err(n, "rooms is too long");

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

          auto * tiledefs = static_cast<context *>(n->ctx)->tiledefs;
          if (!tiledefs->has(cell->atom)) lispy::err(cell, "unknown sprdef");
          data[i * cols + idx] = (*tiledefs)[cell->atom].sprite[0];
        }
      }

      auto rooms = static_cast<context *>(n->ctx)->list;
      rooms->data[as - 1][cols - 1].push_back_doubling(hai::sptr { new t {
        .w = cols,
        .h = as,
        .data = traits::move(data),
      }}); 

      return n;
    };
    
    lispy::run(src, &ctx);

    return rooms;
  }

  export void load(const tiledefs::map & tdefs, jute::view lsp, auto && cb) {
    jojo::read(lsp, nullptr, [&tdefs, cb=traits::move(cb)](auto ptr, hai::cstr & src) {
      cb(run(tdefs, src));
    });
  }
  export void load(const tiledefs::map & tdefs, auto && cb) {
    auto lsp = sires::real_path_name("roomdefs.lsp");
    load(tdefs, lsp, cb);
  }
}
