#pragma leco add_shader gairleog.frag
#pragma leco add_shader gairleog.vert

#ifdef LECO_TARGET_WASM
#pragma leco add_impl v_wasm
#else
#pragma leco add_impl v_vulkan
#endif

export module v;
import dotz;
import glispy;
import hai;
import silog;

namespace v {
  constexpr const auto max_sprites = 10240;

  export struct sprite {
    dotz::vec2 pos;
    dotz::vec2 scale { 1 };
    dotz::vec4 mult { 1 };
    unsigned id;
  };

  export struct grid {
    dotz::vec2 grid_pos;
    dotz::vec2 grid_size;
  };
  export struct mapper {
    virtual ~mapper() {}
    virtual void set_grid(grid g) = 0;
    virtual void push(sprite s) = 0;
  };
  export hai::uptr<mapper> map();

  export hai::fn<void> on_frame = [] {};

  void call_on_frame() {
    auto g = glispy::frame_guard();
    try {
      on_frame();
    } catch (const lispy::parser_error & err) {
      silog::die("%s", lispy::to_file_err(err).begin());
    } catch (const hai::cstr & err) {
      silog::die("%s", err.begin());
    }
  }
}
