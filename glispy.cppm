#pragma leco add_impl glispy_impl
export module glispy;
export import lispy;
import dotz;
import jute;

using namespace lispy;

namespace glispy {
  export auto & game_values() {
    static struct {
      jute::heap level = "0";
      dotz::vec2 perlin {};
    } i;
    return i;
  }

  export lispy::frame_guard frame_guard();
}
