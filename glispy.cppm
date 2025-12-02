export module glispy;
export import lispy;
import jute;

using namespace lispy;

namespace glispy {
  export struct {
    jute::heap level;
  } game_values;

  export auto eval(const node * n) {
    context ctx {};
    ctx.fns["level"] = [](auto n, auto aa, auto as) -> const node * {
      auto nn = clone<node>(n);
      nn->atom = game_values.level;
      return nn;
    };

    auto nn = clone<node>(n);
    nn->ctx = &ctx;
    return eval<node>(&ctx, nn);
  }
}
