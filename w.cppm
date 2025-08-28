export module w;
import casein;
import dotz;
import hai;

namespace v {
  export hai::fn<void> on_frame {};

  export struct sprite {
    dotz::vec2 pos;
    dotz::vec2 uv;
  };
  export class mapper {

  public:
    mapper & operator+=(sprite s) {
      return *this;
    }
  };

  export auto map_buffer() {
    return mapper {
    };
  }
}
