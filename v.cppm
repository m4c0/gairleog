#pragma leco add_shader gairleog.frag
#pragma leco add_shader gairleog.vert

#ifdef LECO_TARGET_WASM
#pragma leco add_impl v_wasm
#else
#pragma leco add_impl v_vulkan
#endif

export module v;
export import :objects;
import glispy;
import hai;
import silog;

namespace v {
  constexpr const auto max_sprites = 10240;

  export class mapper {
  protected:
    grid m_grid {};

    virtual void add_sprite(sprite s) = 0;

  public:
    virtual ~mapper() {}
    void set_grid(grid g) { m_grid = g; }
    void push(sprite s) { 
      static_cast<grid &>(s) = m_grid;
      add_sprite(s);
    }
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
