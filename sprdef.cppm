#pragma leco add_resource_dir sprites
export module sprdef;
import hai;
import hashley;
import jojo;
import jute;
import lispy;
import sires;
import traits;

namespace sprdef {
  [[nodiscard]] auto run(jute::view src) {
    hashley::niamh sprites { 1023 };

    struct custom_node : lispy::node {
      int spr_id;
      bool valid;
    };
    struct context : lispy::context {
      hashley::niamh * spr;
    };

    lispy::ctx_w_mem<custom_node, context> cm {};
    cm.ctx.spr = &sprites;
    cm.ctx.fns["sprdef"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 2) lispy::err(n, "def expects name and value");
      if (!lispy::is_atom(aa[0])) lispy::err(aa[0], "def name must be an atom");
      
      auto spr = static_cast<context &>(ctx).spr;
      (*spr)[aa[0]->atom] = lispy::to_i(eval<custom_node>(ctx, aa[1]));

      return n;
    };
    lispy::run(src, cm.ctx);

    return sprites;
  }

  export void load(jute::view lsp, auto && cb) {
    jojo::read(lsp, nullptr, [cb=traits::move(cb)](auto ptr, hai::cstr & src) {
      cb(run(src));
    });
  }
  export void load(auto && cb) {
    auto lsp = sires::real_path_name("pixelite2.lsp");
    load(lsp, cb);
  }
}
