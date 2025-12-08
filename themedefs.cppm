export module themedefs;
import hai;
import jute;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace themedefs {
  hai::varray<const node *> nodes { 32 };
  auto memory = arena<node>::make();

  export void run(jute::view src) {
    auto m = memory->use();
    each("themedefs.lsp", src, [](auto * n) {
      nodes.push_back_doubling(n);
    });
  }

  export void eval() {
    for (auto n : nodes) auto _ = eval<node>(n);
  }
}
