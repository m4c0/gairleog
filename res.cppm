export module res;
import hai;
import jute;
import roomdefs;
import sires;
import sprdef;
import tiledefs;

namespace res {
  export void on_error(hai::fn<void, jute::view> cb) {
    sires::on_error([cb](auto ptr, auto msg) mutable {
      cb(msg);
    });
  }

  void load(jute::view sprdef_file, void (*cb)()) {
    sprdef::load(sprdef_file, [cb] {
      tiledefs::load([cb] {
        roomdefs::load(cb);
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
