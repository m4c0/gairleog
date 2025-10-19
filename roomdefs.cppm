export module roomdefs;
import entdefs;
import hai;
import hashley;
import jute;
import lispy;
import rng;
import sprdef;
import themedefs;
import tiledefs;
import traits;

namespace roomdefs {
  export struct t {
    unsigned w {};
    unsigned h {};
    hai::array<tiledefs::t> d {};
    tiledefs::t (*fn)(const t & t, unsigned x, unsigned y) {};
  };

  hai::cstr g_src {};
  export void run(jute::view src) { g_src = src.cstr(); }

  struct node : lispy::node {
    hai::sptr<roomdefs::t> room {};
  };
  struct context : lispy::context {
    unsigned w {};
    unsigned h {};
    const node * theme {};
  };

  export hai::sptr<t> for_size(unsigned ew, unsigned eh) try {
    using namespace lispy::experimental;

    context ctx {
      { .allocator = lispy::allocator<node>() },
      ew, eh,
    }; 
    ctx.fns["room"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as < 2) lispy::err(n, "rooms must have at least two rows");

      auto ctx = static_cast<context *>(n->ctx);

      unsigned cols = aa[0]->atom.size();

      if (!(as == ctx->h && cols == ctx->w) &&
          !(as == ctx->w && cols == ctx->h)) return {};

      basic_context<node> tctx {};
      tctx.parent = n->ctx;
      themedefs::eval(&tctx);

      hai::array<tiledefs::t> data { as * cols };
      for (auto i = 0; i < as; i++) {
        if (!lispy::is_atom(aa[i])) lispy::err(aa[i], "all rows must be atoms");
        auto a = aa[i]->atom;
        if (cols != a.size()) lispy::err(aa[i], "all rows must have the same length");
        for (auto idx = 0; idx < cols; idx++) {
          auto c = a.subview(idx, 1).middle;
          if (!tctx.defs.has(c)) lispy::err(aa[i], "unknown def", idx);

          auto cell = lispy::eval<node>(&tctx, tctx.defs[c]);
          if (!lispy::is_atom(cell)) lispy::err(aa[i], "cell must be a sprite name", idx);
          if (!entdefs::has(cell->atom)) lispy::err(cell, "unknown entdef");
          data[i * cols + idx] = entdefs::get(cell->atom);
        }
      }

      hai::sptr r { new t {
        .w = cols,
        .h = as,
        .d = traits::move(data),
      }}; 
      if (ctx->w == r->w) {
        switch (rng::rand(3)) {
          case 0:
            r->fn = [](auto t, auto x, auto y) {
              return t.d[y * t.w + x];
            };
            break;
          case 1:
            r->fn = [](auto t, auto x, auto y) {
              return t.d[y * t.w + (t.w - x - 1)];
            };
            break;
          case 2:
            r->fn = [](auto t, auto x, auto y) {
              return t.d[(t.h - y - 1) * t.w + x];
            };
            break;
          case 3:
            r->fn = [](auto t, auto x, auto y) {
              return t.d[(t.h - y - 1) * t.w + (t.w - x - 1)];
            };
            break;
        }
      } else {
        switch (rng::rand(3)) {
          case 0:
            r->fn = [](auto t, auto y, auto x) {
              return t.d[y * t.w + x];
            };
            break;
          case 1:
            r->fn = [](auto t, auto y, auto x) {
              return t.d[y * t.w + (t.w - x - 1)];
            };
            break;
          case 2:
            r->fn = [](auto t, auto y, auto x) {
              return t.d[(t.h - y - 1) * t.w + x];
            };
            break;
          case 3:
            r->fn = [](auto t, auto y, auto x) {
              return t.d[(t.h - y - 1) * t.w + (t.w - x - 1)];
            };
            break;
        }
      }
      auto nn = clone<node>(n);
      nn->room = r;
      return nn;
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
    return (n && n->room) ? n->room : hai::sptr<t> {};
  } catch (const lispy::parser_error & e) {
    throw lispy::to_file_err("roomdefs.lsp", e);
  }
}
