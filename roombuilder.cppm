export module roombuilder;
import dotz;

namespace roombuilder {
  export bool supports(dotz::ivec2 size) {
    return size.x == 5 && size.y == 3;
  }
}
