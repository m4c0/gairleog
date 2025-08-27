export module sprdef;
import hai;
import hashley;
import lispy;
import sires;

namespace sprdef {
  export [[nodiscard]] auto load() {
    struct custom_node : lispy::node {
      int spr_id;
      bool valid;
    };
    lispy::ctx_w_mem<custom_node> cm {};
    cm.ctx.fns["sprdef"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 2) lispy::err(n, "def expects name and value");
      if (!lispy::is_atom(aa[0])) lispy::err(aa[0], "def name must be an atom");
      auto * nn = new (ctx.allocator()) custom_node { *aa[0] };
      nn->spr_id = lispy::to_i(eval(ctx, aa[1]));
      nn->valid = true;
      return nn;
    };
    hashley::niamh sprites { 1023 };
    auto lsp = sires::real_path_name("pixelite2.lsp");
    lispy::run(lsp, cm.ctx, [&](auto * node) {
      auto nn = static_cast<const custom_node *>(node);
      if (nn->valid) sprites[nn->atom] = nn->spr_id;
    });

    return sprites;
  }
}
