#pragma leco add_resource_dir sprites
export module sprdef;
import hai;
import hashley;
import jute;
import lispy;
import sires;
import traits;

namespace sprdef {
  export using map = hashley::niamh;

  [[nodiscard]] auto run(jute::view src) {
    map sprites { 1023 };

    struct custom_node : lispy::node {
      int spr_id;
      bool valid;
    };
    struct context : lispy::context {
      map * spr;
    } ctx {
      { .allocator = lispy::allocator<custom_node>() },
    };

    ctx.spr = &sprites;
    ctx.fns["sprdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
      if (as != 2) lispy::err(n, "def expects name and value");
      if (!lispy::is_atom(aa[0])) lispy::err(aa[0], "def name must be an atom");
      
      auto spr = static_cast<context *>(n->ctx)->spr;
      (*spr)[aa[0]->atom] = lispy::to_i(eval<custom_node>(n->ctx, aa[1]));

      return n;
    };
    lispy::run(src, &ctx);

    return sprites;
  }

  export void load(jute::view lsp, auto && cb) {
    sires::read(lsp, nullptr, [cb=traits::move(cb)](auto ptr, hai::cstr & src) {
      cb(run(src));
    });
  }
  export void load(auto && cb) {
    load("pixelite2.lsp", cb);
  }
}
