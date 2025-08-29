#pragma leco add_impl w_gl
export module w;
import casein;
import dotz;
import gelo;
import hai;
import vaselin;

namespace v {
  export hai::fn<void> on_frame = [] {};

  export struct sprite {
    dotz::vec2 pos;
    dotz::vec2 uv;
  };
  hai::varray<sprite> buffer { 10240 };
  export class mapper {

  public:
    mapper & operator+=(sprite s) {
      buffer.push_back(s);
      return *this;
    }
  };

  export auto map_buffer() {
    buffer.truncate(0);
    return mapper {};
  }

  void create_window();
  void render();
}

static void frame(void *) {
  v::on_frame();
  v::render();
  vaselin::request_animation_frame(frame, nullptr);
}

const int i = [] {
  using namespace casein;

  handle(CREATE_WINDOW, v::create_window);
  vaselin::request_animation_frame(frame, nullptr);
  return 0;
}();
