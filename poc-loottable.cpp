#pragma leco test

import jute;
import glispy;
import print;
import save;
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

static void test(int level, sv expected, sv err_msg) {
  save::current_stage = level;
  glispy::reset();
  auto n = eval<node>(context()->def("this"));
  assert(n->atom == expected, err_msg);
}

int main() try {
  temp_arena<node> a {};

  auto g = glispy::frame_guard();
  temp_frame src_ctx {};

  run<node>("loot-table-example", src);
  if (!src_ctx.defs.has("this")) die("missing this");

  lispy::temp_frame ctx {};

  test(3, "B", "failed on at-range test");
  test(4, "C", "failed on range test");
  test(8, "D", "failed on fallback test");
} catch (const lispy::parser_error & e) {
  putln("line ", e.line, " col ", e.col, " -- ", e.msg);
}
