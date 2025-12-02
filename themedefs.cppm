export module themedefs;
import hai;
import jute;
import lispy;

using namespace lispy;
using namespace lispy::experimental;

namespace themedefs {
  hai::cstr g_src {};
  export void run(jute::view src) { g_src = src.cstr(); }

  export void eval() try {
    lispy::run<node>(g_src);
  } catch (const lispy::parser_error & e) {
    throw lispy::to_file_err("themedefs.lsp", e);
  }
}
