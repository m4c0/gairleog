#pragma leco add_resource_dir "sprites"
module;
#include <stdio.h>

export module res;
import hai;
import jute;
import lispy;
import roomdefs;
import sires;
import sprdef;
import tiledefs;

namespace res {
  hai::fn<void, jute::view> err_cb {};

  export void on_error(hai::fn<void, jute::view> cb) {
    sires::on_error([cb](auto ptr, auto msg) mutable {
      cb(msg);
    });
  }

  static void report(jute::view file, const lispy::parser_error & e) {
    char msg[128] {};
    auto len = snprintf(msg, sizeof(msg), "%.*s:%d:%d: %.*s",
        static_cast<unsigned>(file.size()), file.begin(),
        e.line, e.col,
        static_cast<unsigned>(e.msg.size()), e.msg.begin());
    if (len > 0) err_cb(jute::view { msg, static_cast<unsigned>(len) });
    else err_cb(e.msg);
  }

  static void load(jute::view sprdef_file, hai::fn<void> cb) {
    sires::read(sprdef_file, nullptr, [=](auto ptr, hai::cstr & src) mutable {
      try {
        sprdef::run(src);
        tiledefs::load([cb] {
          roomdefs::load(cb);
        });
      } catch (const lispy::parser_error & e) {
        report(sprdef_file, e);
      }
    });
  }
  export void load_all(void (*cb)()) {
    load("pixelite2.lsp", cb);
  }
  export void load_locally(void (*cb)()) {
    load("sprites/pixelite2.lsp", cb);
  }
}
