module glispy;

namespace glispy {
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
    return ctx;
  }();

  lispy::frame_guard frame_guard() {
    return lispy::frame_guard { g_ctx };
  }
}
