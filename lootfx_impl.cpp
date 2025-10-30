module lootfx;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace lootfx {
  void run(jute::view src) try {
    basic_context<node> ctx {};
    ctx.fns["fx"] = [](auto n, auto aa, auto as) -> const node * {
      return n;
    };

    ctx.run(src);
  } catch (const parser_error & e) {
    throw to_file_err("lootfx.lsp", e);
  }
}
