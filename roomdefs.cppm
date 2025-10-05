export module roomdefs;
import hai;
import hashley;
import jute;
import lispy;
import rng;
import sprdef;
import traits;

namespace roomdefs {
  export struct error : lispy::parser_error {};

  export struct tiledef {
    bool block {};
    float light {};
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

  static float to_light(const lispy::node * n) {
    auto i = lispy::to_f(n);
    if (i < 0 || i > 1) lispy::err(n, "light intensity should be between 0 and 1");
    return i;
  }
  static unsigned to_spr(const lispy::node * name) {
    if (!lispy::is_atom(name)) lispy::err(name, "spr expects atom as name");
    if (!sprdef::has(name->atom)) lispy::err(name, "invalid sprite name");
    return sprdef::get(name->atom);
  }

  struct node : lispy::node, tiledef {
    void (*attr)(node *, const node *);
    hai::sptr<t> room {};
    bool has_tdef;
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
    ctx.fns["tile"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      basic_context<node> ctx { n->ctx->allocator };
      ctx.fns["block"] = [](auto n, auto aa, auto as) -> const lispy::node * {
        if (as != 0) lispy::err(n, "not expecting a parameter");
        auto * nn = clone<node>(n);
        nn->block = true;
        return nn;
      };
      ctx.fns["light"] = mem_fn<&node::attr, &node::light,  to_light>;
      ctx.fns["spr"]   = mem_fn<&node::attr, &node::sprite, to_spr>;
      auto * nn = fill_clone<node>(&ctx, n, aa, as);
      nn->has_tdef = true;
      return nn;
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
          if (!tdn->has_tdef) lispy::err(tdn, "expecting a tiledef");

          data[i * cols + idx] = *tdn;
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
    throw error { e };
  }
}
