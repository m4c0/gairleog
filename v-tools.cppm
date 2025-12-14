export module v:tools;
import hai;
import sv;
import vinyl;

#ifndef LECO_TARGET_WASM
import vee;
#endif

struct app_stuff;
struct ext_stuff;
using vv = vinyl::v<app_stuff, ext_stuff>;

namespace v {
  struct texture {
    virtual ~texture() {}

    [[nodiscard]] virtual explicit operator bool() const = 0;
  };
  hai::uptr<texture> load_texture(sv name);
};
