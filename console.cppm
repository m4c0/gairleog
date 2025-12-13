export module console;
import dotz;
import hai;
import jute;
import sitime;
import sprdef;

namespace console {
  export constexpr const auto lines = 6;

  struct message {
    float start {};
    jute::heap text {};
    int count {};
  };
  hai::array<message> messages { lines };

  sitime::stopwatch timer {};

  export void reset() {
    for (auto & m : messages) m = {};
  }

  export void push(jute::heap msg) {
    auto & last = messages[messages.size() - 1];
    if (last.text == msg) {
      last.count++;
      return;
    }

    for (auto i = 0; i < messages.size() - 1; i++) {
      messages[i] = messages[i + 1];
    }
    last = message {
      .start = timer.secs(),
      .text = msg,
      .count = 1,
    };
  }

  export void for_each(auto && fn) {
    for (auto m : messages) {
      auto dt = timer.secs() - m.start;
      switch (m.count) {
        case 0: fn(0, jute::view {}); break;
        case 1: fn(dt, *m.text); break;
        default: fn(dt, *jute::fmt<"%s (%d times)">(m.text, m.count)); break;
      }
    }
  }
}
