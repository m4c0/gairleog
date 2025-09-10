#pragma leco test
#pragma leco add_resource "tiledefs.lsp"

import hai;
import hashley;
import jojo;
import lispy;
import print;

struct tiledef {
  unsigned light {};
  hai::array<unsigned> sprite {};
};

int main() try {
  auto src = jojo::read_cstr("tiledefs.lsp");

  struct context : lispy::context {
    hashley::fin<tiledef> tiledefs { 127 };
  } ctx {
    { .allocator = lispy::allocator<>() },
  };
  ctx.fns["tiledef"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as < 2) lispy::err(n, "tiledef requires at least name and spr");
    if (!lispy::is_atom(aa[0])) lispy::err(n, "tiledef name should be an atom");
    return n;
  };
  lispy::run(src, ctx);
} catch (const lispy::parser_error & e) {
  errln("tiledefs.lsp:", e.line, ":", e.col, ": ", e.msg);
  return 1;
}
