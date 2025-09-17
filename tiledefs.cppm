#pragma leco add_resource "tiledefs.lsp"
export module tiledefs;

import hai;
import hashley;
import jute;
import lispy;
import print;
import sires;
import sprdef;

namespace tiledefs {
  export struct t {
    bool block;
    int light {};
    hai::array<unsigned> sprite {};
  };
  auto & map() {
    static hashley::fin<tiledefs::t> i { 127 };
    return i;
  }
  export bool has(jute::view key) { return map().has(key); }
  export const auto & get(jute::view key) { return map()[key]; }
}

static void run(jute::view src) {
  struct node : lispy::node {
    enum { t_empty, t_block, t_light, t_spr } type {};
    tiledefs::t tdef {};
  };
  lispy::context ctx {
    .allocator = lispy::allocator<node>(),
  };

  // static constexpr const auto clone = lispy::clone<node>;
  static constexpr const auto eval = lispy::eval<node>;

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

    auto & t = tiledefs::map()[aa[0]->atom];
    for (auto i = 1; i < as; i++) {
      auto a = eval(n->ctx, aa[i]);
      switch (a->type) {
        case node::t_empty:
          lispy::err(aa[i], "expecting spr, light or block");
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
  lispy::run(src, &ctx);
}

namespace tiledefs {
  export void load(hai::fn<void> cb) {
    sires::read("tiledefs.lsp", nullptr, [cb](auto ptr, hai::cstr & src) mutable {
      run(src);
      cb();
    });
  }
}
