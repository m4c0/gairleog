#pragma leco add_impl glispy_impl
export module glispy;
export import lispy;
import dotz;
import jute;

using namespace lispy;

namespace glispy {
  export void reset();

  export lispy::frame_guard frame_guard();
}
