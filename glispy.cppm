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

  export auto eval(const node * n) {
    context ctx {};
    ctx.fns["level"] = [](auto n, auto aa, auto as) -> const node * {
      auto nn = clone<node>(n);
      nn->atom = game_values().level;
      return nn;
    };

    auto nn = clone<node>(n);
    nn->ctx = &ctx;
    return eval<node>(&ctx, nn);
  }

  static const node * eval_range_table(unsigned val, const node * const * aa, unsigned as) {
    struct c : context {
      unsigned val;
    } ctx;
    ctx.fns["lte"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 2) erred(n, "expecting two atoms (random threshold and picking value)");

      auto thr = to_i(aa[0]); auto val = static_cast<c *>(n->ctx)->val;
      return val <= thr ? aa[1] : nullptr;
    };

    for (auto i = 0; i < as - 1; i++) {
      c sctx {};
      sctx.parent = &ctx;
      sctx.val = val;

      auto ai = clone<node>(aa[i]);
      ai->ctx = &sctx;

      auto n = eval<node>(&sctx, ai);
      if (n) return n;
    }
    c sctx {};
    sctx.parent = &ctx;
    sctx.val = val;
    auto ai = clone<node>(aa[as - 1]);
    ai->ctx = &sctx;
    return eval<node>(&sctx, ai);
  }

  export void setup(context * ctx) {
    ctx->fns["range-table"] = [](auto n, auto aa, auto as) -> const node * {
      if (as < 2) erred(n, "range-table expects at least a variable and the fallback value");
      auto val = to_i(glispy::eval(aa[0]));
      auto res = eval_range_table(val, aa + 1, as - 1);
      return eval<node>(n->ctx, res);
    };
  }
}
