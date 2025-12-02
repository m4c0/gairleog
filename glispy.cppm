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
    temp_frame ctx {};
    ctx.fns["level"] = [](auto n, auto aa, auto as) -> const node * {
      auto nn = clone<node>(n);
      nn->atom = game_values().level;
      return nn;
    };
    return eval<node>(n);
  }

  static const node * eval_range_table(unsigned val, const node * const * aa, unsigned as) {
    temp_frame ctx {};
    ctx.ptrs["val"] = &val;
    ctx.fns["lte"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 2) erred(n, "expecting two atoms (random threshold and picking value)");

      auto thr = to_i(aa[0]);
      auto val = *static_cast<unsigned *>(context()->ptr("val"));
      return val <= thr ? aa[1] : nullptr;
    };

    for (auto i = 0; i < as - 1; i++) {
      temp_frame ctx {};
      auto n = eval<node>(aa[i]);
      if (n) return n;
    }
    return eval<node>(aa[as - 1]);
  }

  export void setup(frame * ctx) {
    ctx->fns["range-table"] = [](auto n, auto aa, auto as) -> const node * {
      if (as < 2) erred(n, "range-table expects at least a variable and the fallback value");
      auto val = to_i(glispy::eval(aa[0]));
      auto res = eval_range_table(val, aa + 1, as - 1);
      return eval<node>(res);
    };
  }
}
