export module imgui;
import dotz;
import hai;
import sv;
import v;

namespace imgui {
  using node = v::sprite;
  struct state {
    dotz::vec2 pos {};
    dotz::vec2 delta {};
    dotz::vec2 scale { 1 };
    float mult { 1 };
  } g_state;
  hai::varray<node> g_buffer { 10240 };

  export void start(v::mapper * m, dotz::vec2 pos, auto && fn) {
    g_buffer.truncate(0);
    g_state = { .pos = pos };

    fn();

    for (const auto & s : g_buffer) m->push(s);
  }

  export void box(dotz::vec2 delta, auto && fn) {
    state old = g_state;

    g_state.delta = delta;
    fn();
    
    old.pos = old.pos + old.delta * old.scale;
    g_state = old;
  };
  export void hbox(auto && fn) {
    box({ 1.f, 0.f }, fn);
  };
  export void vbox(auto && fn) {
    box({ 0.f, 1.f }, fn);
  };

  export void mult(float f, auto && fn) {
    auto old_m = g_state.mult;
    g_state.mult = g_state.mult * f;
    fn();
    g_state.mult = old_m;
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
      g_buffer.push_back(node {
        .pos = g_state.pos,
        .scale = g_state.scale,
        .mult = g_state.mult,
        .id = id,
      });
    });
  };
  export void text(unsigned font, sv str) {
    for (auto c : str) {
      sprite(font + c);
    }
  };
  export void number(unsigned font, unsigned n) {
    if (n == 0) return sprite(font + '0');

    char buf[16] {};
    int qty = 0;
    while (n > 0) {
      buf[qty++] = '0' + (n % 10);
      n /= 10;
    }

    for (; qty > 0; qty--) sprite(font + buf[qty - 1]);
  };
}
