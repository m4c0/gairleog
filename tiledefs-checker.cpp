#pragma leco test
#pragma leco add_resource "tiledefs.lsp"

import hai;
import hashley;
import jojo;
import jute;
import lispy;
import print;
import sprdef;

struct tiledef {
  bool block;
  int light {};
  hai::array<unsigned> sprite {};
};

static void run(jute::view src, const hashley::niamh & sprdefs) {
  struct node : lispy::node {
    enum { t_empty, t_block, t_light, t_spr } type {};
    tiledef tdef {};
  };
  struct context : lispy::context {
    hashley::fin<tiledef> tiledefs { 127 };
    const hashley::niamh * sprs = &sprdefs; 
  } ctx {
    { .allocator = lispy::allocator<node>() },
  };

  // static constexpr const auto clone = lispy::clone<node>;
  static constexpr const auto eval = lispy::eval<node>;

  ctx.fns["block"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 0) lispy::err(n, "block does not accept parameters");
    return new (ctx.allocator()) node { *n, node::t_block, { .block = true } };
  };
  ctx.fns["light"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) lispy::err(n, "light requires intensity as parameter");
    auto i = lispy::to_i(eval(ctx, aa[0]));
    if (i < 0 || i > 15) lispy::err(n, "light intensity should be between 0 and 15");
    return new (ctx.allocator()) node { *n, node::t_light, { .light = i } };
  };
  ctx.fns["spr"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as == 0) lispy::err(n, "spr requires at least one name");

    auto & sprs = *static_cast<context &>(ctx).sprs;

    auto nn = new (ctx.allocator()) node { *n, node::t_spr, { .sprite { as } } };
    for (auto i = 0; i < as; i++) {
      if (!lispy::is_atom(aa[i])) lispy::err(aa[i], "spr expects atom names");
      if (!sprs.has(aa[i]->atom)) lispy::err(aa[i], "invalid sprite name");
      nn->tdef.sprite[i] = sprs[aa[i]->atom];
    }
    return nn;
  };
  ctx.fns["tiledef"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as < 2) lispy::err(n, "tiledef requires at least name and spr");
    if (!lispy::is_atom(aa[0])) lispy::err(n, "tiledef name should be an atom");

    auto & t = static_cast<context &>(ctx).tiledefs[aa[0]->atom];
    for (auto i = 1; i < as; i++) {
      auto a = eval(ctx, aa[i]);
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
  lispy::run(src, ctx);
}

int main() try {
  sprdef::load("sprites/pixelite2.lsp", [](auto sprdefs) {
    auto src = jojo::read_cstr("tiledefs.lsp");
    run(src, sprdefs);
  });
} catch (const lispy::parser_error & e) {
  errln("tiledefs.lsp:", e.line, ":", e.col, ": ", e.msg);
  return 1;
}
