#pragma leco add_resource_dir "sprites"
#pragma leco add_resource "hitdefs.lsp"
#pragma leco add_resource "roomdefs.lsp"
module;
#include <stdio.h>

export module res;
import hai;
import hitdefs;
import jute;
import lispy;
import roomdefs;
import sires;
import sprdef;

namespace res {
  hai::fn<void, jute::view> err_cb {};

  export void on_error(hai::fn<void, jute::view> cb) {
    err_cb = cb;
    sires::on_error([cb](auto ptr, auto msg) mutable {
      cb(msg);
    });
  }

#ifndef LECO_TARGET_WASM
  static void report(jute::view file, const lispy::parser_error & e) {
    char msg[128] {};
    auto len = snprintf(msg, sizeof(msg), "%.*s:%d:%d: %.*s",
        static_cast<unsigned>(file.size()), file.begin(),
        e.line, e.col,
        static_cast<unsigned>(e.msg.size()), e.msg.begin());
    if (len > 0) err_cb(jute::view { msg, static_cast<unsigned>(len) });
    else err_cb(e.msg);
  }
#endif

  static void safe_load(jute::view file, hai::fn<void, jute::view> cb) {
    sires::read(file, nullptr, [=](auto ptr, hai::cstr & src) mutable {
#ifdef LECO_TARGET_WASM
      // No exceptions ATM
      cb(src);
#else
      try {
        cb(src);
      } catch (const lispy::parser_error & e) {
        report(file, e);
      }
#endif
    });
  }

  static void load(jute::view sprdef_file, hai::fn<void> cb) {
    safe_load(sprdef_file, [=](auto src) mutable {
      sprdef::run(src);

      safe_load("roomdefs.lsp", [=](auto src) mutable {
        roomdefs::run(src);

        safe_load("hitdefs.lsp", [=](auto src) mutable {
          hitdefs::run(src);

          cb();
        });
      });
    });
  }
  export void load_all(void (*cb)()) {
    load("pixelite2.lsp", cb);
  }
  export void load_locally(void (*cb)()) {
    load("sprites/pixelite2.lsp", cb);
  }
}
