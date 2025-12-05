export module themedefs;
import hai;
import jute;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace themedefs {
  hai::cstr g_src {};
  export void run(jute::view src) { g_src = src.cstr(); }

  // TODO: cache in a context
  export void eval() {
    lispy::run<node>("themedefs.lsp", g_src);
  }
}
