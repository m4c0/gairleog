#pragma leco add_resource_dir "sprites"
#pragma leco add_resource "entdefs.lsp"
#pragma leco add_resource "hitdefs.lsp"
#pragma leco add_resource "lootfx.lsp"
#pragma leco add_resource "roomdefs.lsp"
#pragma leco add_resource "themedefs.lsp"
module;
#include <stdio.h>

export module res;
import entdefs;
import hai;
import hitdefs;
import jojo;
import jojo;
import jute;
import lispy;
import lootfx;
import roomdefs;
import sires;
import sprdef;
import themedefs;

namespace res {
  static void safe_load(jute::view file, hai::fn<void, jute::view> cb) {
    sires::read(file, nullptr, [=](auto ptr, hai::cstr & src) mutable {
      cb(src);
    });
  }

  static void load(jute::view sprdef_file, hai::fn<void> cb) {
    jojo::on_error([](auto ptr, auto msg) {
      throw msg.cstr();
    });
    safe_load(sprdef_file, [=](auto src) mutable {
      sprdef::run(sprdef_file, src);

      safe_load("entdefs.lsp", [=](auto src) mutable {
        entdefs::run(src);

        safe_load("themedefs.lsp", [=](auto src) mutable {
          themedefs::run(src);

          safe_load("roomdefs.lsp", [=](auto src) mutable {
            roomdefs::run(src);

            safe_load("hitdefs.lsp", [=](auto src) mutable {
              hitdefs::run(src);

              safe_load("lootfx.lsp", [=](auto src) mutable {
                lootfx::run(src);

                cb();
              });
            });
          });
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
