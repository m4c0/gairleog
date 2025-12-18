export module v:objects;
import dotz;

namespace v {
  export struct grid {
    dotz::vec2 grid_pos;
    dotz::vec2 grid_size;
  };
  export struct sprite : grid {
    dotz::vec2 pos;
    dotz::vec2 scale { 1 };
    dotz::vec4 mult { 1 };
    float rotation {};
    unsigned id;
  };
}
