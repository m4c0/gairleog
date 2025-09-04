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
  export struct t {
    unsigned w;
    unsigned h;
    hai::array<unsigned> data;
  };

  [[nodiscard]] auto run(const hashley::niamh & sprdefs, jute::view src) {
    struct node : lispy::node {
      hai::sptr<t> room {};
    };
    struct ctx : lispy::context {
      const hashley::niamh * sprdefs;
    }; 
    lispy::ctx_w_mem<node, ctx> cm {};
    cm.ctx.sprdefs = &sprdefs;
    cm.ctx.fns["room"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
      if (as < 2) lispy::err(n, "rooms must have at least two rows");

      unsigned cols = aa[0]->atom.size();
      hai::array<unsigned> data { cols * as };
      for (auto i = 0; i < as; i++) {
        if (!lispy::is_atom(aa[i])) lispy::err(aa[i], "all rows must be atoms");
        auto a = aa[i]->atom;
        if (cols != a.size()) lispy::err(aa[i], "all rows must have the same length");
        for (auto idx = 0; idx < a.size(); idx++) {
          auto c = a.subview(idx, 1).middle;
          if (!ctx.defs.has(c)) lispy::err(aa[i], "unknown def", idx);

          auto cell = lispy::eval(ctx, ctx.defs[c]);
          if (lispy::is_atom(cell)) {
            auto & sprdefs = static_cast<struct ctx &>(ctx).sprdefs;
            if (!sprdefs->has(cell->atom)) lispy::err(cell, "unknown sprdef");
          } else if (cell->next) lispy::err(aa[i], "cell must be a sprite name", idx);
        }
      }

      auto * nn = new (ctx.allocator()) node { *aa[0] };
      return nn;
    };
    
    lispy::run(src, cm.ctx, [](auto * n) {
      [[maybe_unused]]
      auto nn = static_cast<const node *>(n);
    });

    return 0;
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
