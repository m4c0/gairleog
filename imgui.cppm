export module imgui;
import dotz;
import hai;
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
    node n {};

    state old = g_state;
    g_state.delta = delta;
    g_state.tail = &n;
    fn();
    g_state = old;
    adv();
  };
  export inline void hbox(auto && fn) {
    node n {};

    state old = g_state;
    g_state.tail = &n;
    g_state.delta = {};
    fn();

    float x = 0;
    for (auto p = n.next; p; p = p->next) {
      p->spr.pos.x += x;
      x += p->size.x;
    }

    g_state = old;
    adv();
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

    auto * n = &g_buffer.back();
    g_state.tail->next = n;
    g_state.tail = n;
    adv();
  }

  export void space(dotz::vec2 size) {
    compo(node {
      .size = g_state.scale,
    });
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
