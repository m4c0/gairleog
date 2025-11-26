#pragma leco app

import hai;
import jute;
import lispy;
import res;
import silog;
import sires;
import v;

static void on_frame() {
}

using namespace lispy;
using namespace lispy::experimental;

static const node * load_room(const node * n, const node * const * aa, unsigned as) {
  silog::trace(as);
  return n;
}

static const node * load_roomdefs(const node * n, const node * const * aa, unsigned as) {
  context ctx { n->ctx->allocator }; 
  ctx.fns["room"] = load_room;

  for (auto i = 0; i < as; i++) {
    auto _ = eval<node>(&ctx, aa[i]);
  }

  return n;
}

static void load(void *, const hai::cstr & src) try {
  basic_context<node> ctx {}; 
  ctx.fns["roomdefs"] = load_roomdefs;
  run<node>(src, &ctx);

  v::on_frame = on_frame;
} catch (const lispy::parser_error & e) {
  silog::error(lispy::to_file_err("roomdefs.lsp", e));
}

static void on_init() {
  sires::read("roomdefs.lsp", nullptr, load);
}

const int i = [] {
  res::load_all(on_init);
  return 0;
}();
