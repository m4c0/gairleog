export module imgui;
import dotz;
import sv;
import v;

namespace imgui {
  struct state {
    v::mapper * m;
    dotz::vec2 pos {};
    dotz::vec2 delta {};
    dotz::vec2 scale { 1 };
  } g_state;

  export void start(v::mapper * m, auto && fn) {
    g_state = { m };
    fn();
  }

  export void box(dotz::vec2 delta, auto && fn) {
    auto old_delta = g_state.delta;
    auto old_pos = g_state.pos;

    g_state.delta = delta;
    fn();
    
    g_state.delta = old_delta;
    g_state.pos = old_pos + g_state.delta * g_state.scale;
  };
  export void hbox(auto && fn) {
    box({ 1.f, 0.f }, fn);
  };
  export void vbox(auto && fn) {
    box({ 0.f, 1.f }, fn);
  };

  export void scale(dotz::vec2 s, auto && fn) {
    auto old_s = g_state.scale;
    g_state.scale = g_state.scale * s;
    fn();
    g_state.scale = old_s;
  };

  export void space(dotz::vec2 size) {
    scale(size, [&] {
      box({}, [] {});
    });
  };

  export void sprite(unsigned id) {
    box({}, [&] {
      g_state.m->push({
        .pos = g_state.pos,
        .scale = g_state.scale,
        .id = id,
      });
    });
  };
  export void text(unsigned font, sv str) {
    for (auto c : str) {
      sprite(font + c);
    }
  };
}
