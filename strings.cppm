#pragma leco add_resource "strings.lsp"
export module strings;
import hai;
import jute;
import lispy;
import sires;
import sv;

using namespace lispy;

namespace strings {
  auto g_mem = arena<node>::make();
  auto g_ctx = frame::make();

  export void load(hai::fn<void> callback) {
    sires::read("strings.lsp", nullptr, [=](auto, auto & src) mutable {
      auto a = g_mem->use();
      frame_guard c { g_ctx };
      run<node>("strings.lsp", src);
      callback();
    });
  }

  auto g_tmp = arena<node>::make();
  export jute::heap get(sv key) {
    frame_guard c { g_ctx };
    temp_frame ctx {};

    g_tmp->reset();
    auto a = g_tmp->use();

    return jute::heap { eval<node>(context()->def(key))->atom };
  }
}
