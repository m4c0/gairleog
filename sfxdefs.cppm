#pragma leco add_resource_dir sfx
export module sfxdefs;
import errs;
import hai;
import lispy;
import sires;
import sv;

namespace sfxdefs {
  static void run(sv src) {
  }

  export void load(hai::fn<void> && callback) {
    sires::read("sfxdefs.lsp", &errs::jid, [=](auto, hai::cstr & src) mutable {
      run(src);
      callback();
    });
  }
}
