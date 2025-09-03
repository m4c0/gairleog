#pragma leco add_resource roomdefs.lsp
export module roomdefs;
import hai;
import jojo;
import jute;
import sires;
import traits;

namespace roomdefs {
  [[nodiscard]] auto run(jute::view src) {
    return 0;
  }

  export void load(auto && cb) {
    auto lsp = sires::real_path_name("roomdefs.lsp");
    jojo::read(lsp, nullptr, [cb=traits::move(cb)](auto ptr, hai::cstr & src) {
      cb(run(src));
    });
  }
}
