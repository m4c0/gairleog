#ifdef LECO_TARGET_WASM
#pragma leco add_impl v_wasm
#else
#pragma leco add_impl v_vulkan
#endif

export module v;
import dotz;
import hai;

namespace v {
  constexpr const auto max_sprites = 10240;

  export struct sprite {
    dotz::vec2 pos;
    dotz::vec2 uv;
  };

  export struct upc {
    dotz::vec2 grid_pos;
    dotz::vec2 grid_size;
  } pc;

  export struct mapper {
    virtual ~mapper() {}
    virtual void push(sprite s) = 0;
  };
  export hai::uptr<mapper> map();

  export hai::fn<void> on_frame = [] {};
}
