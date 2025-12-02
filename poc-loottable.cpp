#pragma leco test

import jute;
import glispy;
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

int main() try {
  temp_arena<node> a {};

  context src_ctx {};
  run<node>(src, &src_ctx);
  if (!src_ctx.defs.has("this")) die("missing this");

  context ctx { .parent = &src_ctx };
  glispy::setup(&ctx);

  glispy::game_values().level = "3";

  auto n = clone<node>(src_ctx.defs["this"]);
  n->ctx = &ctx;
  assert(eval<node>(&ctx, n)->atom == "B"_sv, "failed on at-range test");

  glispy::game_values().level = "4";

  n = clone<node>(src_ctx.defs["this"]);
  n->ctx = &ctx;
  assert(eval<node>(&ctx, n)->atom == "C"_sv, "failed on range test");

  glispy::game_values().level = "8";

  n = clone<node>(src_ctx.defs["this"]);
  n->ctx = &ctx;
  assert(eval<node>(&ctx, n)->atom == "D"_sv, "failed on fallback test");
} catch (const lispy::parser_error & e) {
  putln("line ", e.line, " col ", e.col, " -- ", e.msg);
}
