export module themedefs;
import hai;
import jute;
import lispy;

namespace themedefs {
  static auto g_arena = lispy::arena<lispy::node>::make();
  static auto g_ctx = lispy::frame::make();

  export void run(jute::view src) {
    auto a = g_arena->use();
    auto c = g_ctx->use();
    lispy::run<lispy::node>("themedefs.lsp", src);
  }

  export auto use() {
    return g_ctx->use();
  }
}
