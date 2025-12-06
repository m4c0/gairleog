export module glispy;
export import lispy;
import jute;

using namespace lispy;

namespace glispy {
  export auto & game_values() {
    static struct {
      jute::heap level = "0";
    } i;
    return i;
  }

  export void init() {
    static auto ctx = frame::make();
    static frame_guard u { ctx };
    ctx->fns["first-of"] = [](auto n, auto aa, auto as) -> const node * {
      for (auto i = 0; i < as; i++) {
        auto nn = eval<node>(aa[i]);
        if (nn) return nn;
      }
      return nullptr;
    };
    ctx->fns["level"] = [](auto n, auto aa, auto as) -> const node * {
      auto nn = clone<node>(n);
      nn->atom = game_values().level;
      return nn;
    };
    ctx->fns["lte"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 3) erred(n, "expecting two values for comparison and one for result");
      auto lhs = to_i(eval<node>(aa[0]));
      auto rhs = to_i(eval<node>(aa[1]));
      return lhs <= rhs ? eval<node>(aa[2]) : nullptr;
    };
    ctx->fns["gte"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 3) erred(n, "expecting two values for comparison and one for result");
      auto lhs = to_i(eval<node>(aa[0]));
      auto rhs = to_i(eval<node>(aa[1]));
      return lhs >= rhs ? eval<node>(aa[2]) : nullptr;
    };
  }
}
