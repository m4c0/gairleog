export module glispy;
export import lispy;
import jute;

using namespace lispy;

namespace glispy {
  export struct {
    jute::heap level;
  } game_values;
  export inline void setup_game_context(context * ctx) {
    ctx->fns["level"] = [](auto n, auto aa, auto as) -> const node * {
      auto nn = clone<node>(n);
      nn->atom = game_values.level;
      return nn;
    };
  }
}
