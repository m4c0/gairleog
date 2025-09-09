#pragma leco add_resource roomdefs.lsp
export module roomdefs;
import hai;
import hashley;
import jojo;
import jute;
import lispy;
import sires;
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

  [[nodiscard]] auto run(const hashley::niamh & sprdefs, jute::view src) {
    struct node : lispy::node {
      hai::sptr<t> room {};
    };
    struct context : lispy::context {
      const hashley::niamh * sprdefs;
      list * list;
    }; 

    list rooms {};

    lispy::ctx_w_mem<node, context> cm {};
    cm.ctx.sprdefs = &sprdefs;
    cm.ctx.list = &rooms;
    cm.ctx.fns["room"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
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
          if (!ctx.defs.has(c)) lispy::err(aa[i], "unknown def", idx);

          auto cell = lispy::eval<node>(ctx, ctx.defs[c]);
          if (!lispy::is_atom(cell)) lispy::err(aa[i], "cell must be a sprite name", idx);

          auto * sprdefs = static_cast<context &>(ctx).sprdefs;
          if (!sprdefs->has(cell->atom)) lispy::err(cell, "unknown sprdef");
          data[i * cols + idx] = (*sprdefs)[cell->atom];
        }
      }

      auto rooms = static_cast<context &>(ctx).list;
      rooms->data[as - 1][cols - 1].push_back_doubling(hai::sptr { new t {
        .w = cols,
        .h = as,
        .data = traits::move(data),
      }}); 

      return n;
    };
    
    lispy::run(src, cm.ctx);

    return rooms;
  }

  export void load(const hashley::niamh & sprdefs, jute::view lsp, auto && cb) {
    jojo::read(lsp, nullptr, [&sprdefs, cb=traits::move(cb)](auto ptr, hai::cstr & src) {
      cb(run(sprdefs, src));
    });
  }
  export void load(const hashley::niamh & sprdefs, auto && cb) {
    auto lsp = sires::real_path_name("roomdefs.lsp");
    load(sprdefs, lsp, cb);
  }
}
