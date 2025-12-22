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
import sv;

namespace roomdefs {
  export struct t {
    unsigned w {};
    unsigned h {};
    hai::array<entdefs::t> d {};
    entdefs::t (*fn)(const t & t, unsigned x, unsigned y) {};
  };

  struct def {
    unsigned w;
    unsigned h;
    hai::array<const lispy::node *> rows;
  };
  hai::varray<def> rooms { 128 };

  export void run(jute::view src) {
    lispy::temp_frame ctx {};
    ctx.fns["roomdefs"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as == 0) lispy::erred(n, "missing at least one room option");

      lispy::temp_frame ctx {};
      ctx.fns["room"] = [](auto n, auto aa, auto as) -> const lispy::node * {
        if (as < 2) lispy::erred(n, "rooms must have at least two rows");

        unsigned cols = aa[0]->atom.size();

        def d {
          .w = cols,
          .h = as,
          .rows { as },
        };
        for (auto i = 0; i < as; i++) {
          if (!lispy::is_atom(aa[i])) lispy::erred(aa[i], "all rows must be atoms");
          auto a = aa[i]->atom;
          if (cols != a.size()) lispy::erred(aa[i], "all rows must have the same length");
          d.rows[i] = aa[i];
        }
        rooms.push_back(d);

        return n;
      };

      rooms.truncate(0);
      rooms.set_capacity(as);
      for (auto i = 0; i < as; i++) auto _ = lispy::eval<lispy::node>(aa[i]);
      return n;
    };

    static auto arena = lispy::arena<lispy::node>::make();
    auto a = arena->use();
    lispy::run<lispy::node>("roomdefs.lsp", src);
  }

  export hai::sptr<t> for_size(unsigned ew, unsigned eh) {
    hai::varray<const def *> opts { rooms.size() };
    for (auto & d : rooms) {
      if (!(d.w == eh && d.h == ew) &&
          !(d.w == ew && d.h == eh)) continue;
      opts.push_back(&d);
    }
    if (opts.size() == 0) return {};
    auto * d = opts[rng::rand(opts.size())];

    lispy::temp_arena<lispy::node> a {};

    hai::array<entdefs::t> data { d->w * d->h };
    for (auto y = 0; y < d->h; y++) {
      auto row = d->rows[y];
      for (auto x = 0; x < d->w; x++) {
        auto c = row->atom.subview(x, 1).middle;

        lispy::temp_frame ctx {};
        themedefs::eval();

        auto n = lispy::context()->def(c);
        if (!n) lispy::erred(row, "unknown def", x);

        auto cell = lispy::eval<lispy::node>(n);
        if (!lispy::is_atom(cell)) lispy::erred(row, "cell must be an entdef name", x);
        if (!entdefs::has(cell->atom)) lispy::erred(cell, "unknown entdef");
        data[y * d->w + x] = entdefs::get(cell->atom);
      }
    }

    {
      hai::sptr r { new t {
        .w = d->w,
        .h = d->h,
        .d = traits::move(data),
      }}; 
      if (ew == r->w) {
        switch (rng::rand_i(3)) {
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
        switch (rng::rand_i(3)) {
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
      return r;
    }
  }
}
