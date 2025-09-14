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
  export using map = hashley::fin<tiledefs::t>;
}

static auto run(jute::view src, const hashley::niamh & sprdefs) {
  struct node : lispy::node {
    enum { t_empty, t_block, t_light, t_spr } type {};
    tiledefs::t tdef {};
  };
  struct context : lispy::context {
    tiledefs::map tiledefs { 127 };
    const hashley::niamh * sprs = &sprdefs; 
  } ctx {
    { .allocator = lispy::allocator<node>() },
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

    auto & sprs = *static_cast<context *>(n->ctx)->sprs;

    auto nn = new (n->ctx->allocator()) node { *n, node::t_spr, { .sprite { as } } };
    for (auto i = 0; i < as; i++) {
      if (!lispy::is_atom(aa[i])) lispy::err(aa[i], "spr expects atom names");
      if (!sprs.has(aa[i]->atom)) lispy::err(aa[i], "invalid sprite name");
      nn->tdef.sprite[i] = sprs[aa[i]->atom];
    }
    return nn;
  };
  ctx.fns["tiledef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
    if (as < 2) lispy::err(n, "tiledef requires at least name and spr");
    if (!lispy::is_atom(aa[0])) lispy::err(n, "tiledef name should be an atom");

    auto & t = static_cast<context *>(n->ctx)->tiledefs[aa[0]->atom];
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

  return traits::move(ctx.tiledefs);
}

namespace tiledefs {
  export void load(const hashley::niamh & spr, auto && cb) {
    sires::read("tiledefs.lsp", nullptr, [cb=traits::move(cb),&spr](auto ptr, hai::cstr & src) {
      cb(run(src, spr));
    });
  }
}
