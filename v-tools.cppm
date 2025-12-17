export module v:tools;
import :objects;
import hai;
import sv;
import vinyl;

struct app_stuff;
struct ext_stuff;
using vv = vinyl::v<app_stuff, ext_stuff>;

namespace v {
  struct base_app_stuff : vinyl::base_app_stuff {
    vinyl::nearest_texture txt { "pixelite2.png" };
    vinyl::vert_shader vert { "gairleog", [] {} };
    vinyl::frag_shader frag { "gairleog", [] {} };

    base_app_stuff() : vinyl::base_app_stuff("Gairleog") {}

    [[nodiscard]] operator bool() const { return txt && vert && frag; }
  };
}
