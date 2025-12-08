export module imgui;
import dotz;
import hai;
import sitime;
import sv;
import v;

namespace imgui {
  struct node {
    v::sprite spr {};
    dotz::vec2 size { 1 };
  };
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

    for (const auto & s : g_buffer) if (s.spr.id) m->push(s.spr);
  }

  static inline void adv(dotz::vec2 s = { 1 }) {
    g_state.pos = g_state.pos + g_state.delta * g_state.scale * s;
  }

  export inline void box(dotz::vec2 delta, auto && fn) {
    state old = g_state;
    g_state.delta = delta;
    fn();
    g_state = old;
    adv();
  };
  export inline void hbox(auto && fn) {
    box({ 1.f, 0.f }, fn);
  };
  export inline void vbox(auto && fn) {
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

  static inline void compo(node no) {
    g_buffer.push_back(no);
    adv();
  }

  export void space() {
    compo({});
  };

  export void sprite(unsigned id) {
    compo(node {
      .spr {
        .pos = g_state.pos,
        .scale = g_state.scale,
        .mult = g_state.mult,
        .id = id,
      },
      .size = g_state.scale,
    });
  }

  export void text(unsigned font, sv str) {
    static sitime::stopwatch timer {};

    bool blink = false;
    for (auto i = 0; i < str.size(); i++) {
      auto c = str[i];

      if (c == 0x1B && i < str.size() - 3) {
        if (str[i + 1] == '[') {
          if (str[i + 2] == '5') {
            if (str[i + 3] == 'm') {
              blink = true;
              i += 3;
              continue;
            }
          }
        }
      }

      float a = blink 
        ? dotz::sin(timer.secs() * 3) * 0.5 + 0.5
        : 1;
      mult(a, [&] { sprite(font + c); });
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
