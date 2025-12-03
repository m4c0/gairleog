#pragma leco test

import jute;
import glispy;
import print;
import sv;

using namespace lispy;

static constexpr const auto src = R"(
  (def this (first-of
    (lte (level) 1 (def that A))
    (lte (level) 3 (def that B))
    (lte (level) 5 (def that C))
    (def that D)
  ))
)"_sv;

int main() try {
  temp_arena<node> a {};

  temp_frame src_ctx {};
  run<node>(src);
  if (!src_ctx.defs.has("this")) die("missing this");

  temp_frame ctx {};
  glispy::setup(&ctx);

  glispy::game_values().level = "3";
  auto _ = eval<node>(context()->def("this"));
  auto n = context()->def("that");
  assert(n, "failed to calculate on at-range test");
  assert(n->atom == "B"_sv, "failed on at-range test");

  glispy::game_values().level = "4";
  auto _ = eval<node>(context()->def("this"));
  n = context()->def("that");
  assert(n, "failed to calculate on range test");
  assert(n && n->atom == "C"_sv, "failed on range test");

  glispy::game_values().level = "8";
  auto _ = eval<node>(context()->def("this"));
  n = context()->def("that");
  assert(n, "failed to calculate on fallback test");
  assert(n && n->atom == "D"_sv, "failed on fallback test");
} catch (const lispy::parser_error & e) {
  putln("line ", e.line, " col ", e.col, " -- ", e.msg);
}
