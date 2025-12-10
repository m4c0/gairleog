#pragma leco add_impl glispy_impl
export module glispy;
export import lispy;
import jute;
import perlin;

using namespace lispy;

namespace glispy {
  export auto & game_values() {
    static struct {
      jute::heap level = "0";
      jute::heap perlin_x = "0";
      jute::heap perlin_y = "0";
    } i;
    return i;
  }

  export lispy::frame_guard frame_guard();
}
