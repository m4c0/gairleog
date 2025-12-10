module glispy;
import perlin;

namespace glispy {
  using nt = const node *;
  
  static perlin g_perlin {};

  static nt perlin(nt n, nt const * aa, unsigned as) {
    if (as == 0) erred(n, "perlin expects at least one value");

    auto f = g_perlin(game_values().perlin) * 0.5 + 0.5;
    auto i = static_cast<int>(f * as);
    return aa[i];
  }

  static auto g_ctx = [] {
    auto ctx = frame::make();
    ctx->fns["first-of"] = [](auto n, auto aa, auto as) -> const node * {
      for (auto i = 0; i < as; i++) {
        auto nn = eval<node>(aa[i]);
        if (nn) return nn;
      }
      return nullptr;
    };
    ctx->fns["level"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 0) erred(n, "level does not take parameters");
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
    ctx->fns["perlin"] = perlin;
    return ctx;
  }();

  lispy::frame_guard frame_guard() {
    return lispy::frame_guard { g_ctx };
  }
}
