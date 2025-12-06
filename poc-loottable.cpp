#pragma leco test

import jute;
import glispy;
import print;
import sv;

using namespace lispy;

static constexpr const auto src = R"(
  (def this (first-of
    (lte (level) 1 A)
    (lte (level) 3 B)
    (lte (level) 5 C)
    D
  ))
)"_sv;

int main() try {
  temp_arena<node> a {};

  auto g = glispy::frame_guard();
  temp_frame src_ctx {};

  run<node>("loot-table-example", src);
  if (!src_ctx.defs.has("this")) die("missing this");

  lispy::temp_frame ctx {};

  glispy::game_values().level = "3";
  auto n = eval<node>(context()->def("this"));
  assert(n->atom == "B"_sv, "failed on at-range test");

  glispy::game_values().level = "4";
  n = eval<node>(context()->def("this"));
  assert(n->atom == "C"_sv, "failed on range test");

  glispy::game_values().level = "8";
  n = eval<node>(context()->def("this"));
  assert(n->atom == "D"_sv, "failed on fallback test");
} catch (const lispy::parser_error & e) {
  putln("line ", e.line, " col ", e.col, " -- ", e.msg);
}
