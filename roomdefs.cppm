export module roomdefs;
import entdefs;
import glispy;
import hai;
import hashley;
import jute;
import lispy;
import rng;
import sprdef;
import themedefs;
import traits;

namespace roomdefs {
  export struct t {
    unsigned w {};
    unsigned h {};
    hai::array<entdefs::t> d {};
    entdefs::t (*fn)(const t & t, unsigned x, unsigned y) {};
  };

  hai::cstr g_src {};
  export void run(jute::view src) { g_src = src.cstr(); }

  struct node : lispy::node {
    hai::sptr<roomdefs::t> room {};
  };

  export hai::sptr<t> for_size(unsigned ew, unsigned eh) try {
    using namespace lispy::experimental;

    lispy::temp_arena<node> a {};
    lispy::temp_frame ctx {};
    glispy::setup(&ctx);
    ctx.ptrs["ew"] = &ew;
    ctx.ptrs["eh"] = &eh;
    ctx.fns["room"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as < 2) lispy::erred(n, "rooms must have at least two rows");

      auto ew = *static_cast<unsigned *>(lispy::context()->ptr("ew"));
      auto eh = *static_cast<unsigned *>(lispy::context()->ptr("eh"));

      unsigned cols = aa[0]->atom.size();

      if (!(as == eh && cols == ew) &&
          !(as == ew && cols == eh)) return {};

      lispy::temp_frame tctx {};
      glispy::setup(&tctx);
      themedefs::eval();

      hai::array<entdefs::t> data { as * cols };
      for (auto i = 0; i < as; i++) {
        if (!lispy::is_atom(aa[i])) lispy::erred(aa[i], "all rows must be atoms");
        auto a = aa[i]->atom;
        if (cols != a.size()) lispy::erred(aa[i], "all rows must have the same length");
        for (auto idx = 0; idx < cols; idx++) {
          auto c = a.subview(idx, 1).middle;
          if (!tctx.defs.has(c)) lispy::erred(aa[i], "unknown def", idx);

          auto cell = lispy::eval<node>(tctx.defs[c]);
          if (!lispy::is_atom(cell)) lispy::erred(aa[i], "cell must be a sprite name", idx);
          if (!entdefs::has(cell->atom)) lispy::erred(cell, "unknown entdef");
          data[i * cols + idx] = entdefs::get(cell->atom);
        }
      }

      hai::sptr r { new t {
        .w = cols,
        .h = as,
        .d = traits::move(data),
      }}; 
      if (ew == r->w) {
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
      if (as == 0) lispy::erred(n, "missing at least one room option");

      hai::varray<const node *> opts { as };
      for (auto i = 0; i < as; i++) {
        auto a = lispy::eval<node>(aa[i]);
        if (a) opts.push_back(a);
      }
      if (opts.size() == 0) return {};
      return opts[rng::rand(opts.size())];
    };
    
    auto n = lispy::run<node>(g_src);
    return (n && n->room) ? n->room : hai::sptr<t> {};
  } catch (const lispy::parser_error & e) {
    throw lispy::to_file_err("roomdefs.lsp", e);
  }
}
