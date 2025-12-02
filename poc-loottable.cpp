#pragma leco test

import lispy;
import print;
import sv;

using namespace lispy;

static constexpr const auto src = R"(
  (def this (range-table (level)
    (lte 1 A)
    (lte 3 B)
    (lte 5 C)
    D
  ))
)"_sv;

static auto table_var_ctx = [] {
  context ctx {};
  ctx.fns["level"] = [](auto n, auto aa, auto as) -> const node * {
    auto nn = clone<node>(n);
    nn->atom = "8";
    return nn;
  };
  return ctx;
}();

static auto eval_table_var(const node * n) {
  context ctx { .parent = &table_var_ctx };
  auto nn = clone<node>(n);
  nn->ctx = &ctx;
  return to_i(eval<node>(&ctx, nn));
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

int main() try {
  temp_arena<node> a {};

  context src_ctx {};
  run<node>(src, &src_ctx);
  if (!src_ctx.defs.has("this")) die("missing this");

  context ctx { .parent = &src_ctx };
  ctx.fns["range-table"] = [](auto n, auto aa, auto as) -> const node * {
    if (as < 2) erred(n, "random-table expects at least a variable and the fallback value");
    auto val = eval_table_var(aa[0]);
    auto res = eval_range_table(val, aa + 1, as - 1);
    return eval<node>(n->ctx, res);
  };

  auto n = clone<node>(src_ctx.defs["this"]);
  n->ctx = &ctx;
  assert(eval<node>(&ctx, n)->atom == "D"_sv, "failed on fallback test");
} catch (const lispy::parser_error & e) {
  putln("line ", e.line, " col ", e.col, " -- ", e.msg);
}
