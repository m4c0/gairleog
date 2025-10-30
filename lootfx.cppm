export module lootfx;
import hai;

namespace lootfx {
  export struct fx {
    bool valid;
  };

  hai::array<fx> map { 1024 };

  export fx & for_sprite(int s) { return map[s]; }
}
