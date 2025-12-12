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
  };
  hai::array<message> messages { lines };

  sitime::stopwatch timer {};

  export void reset() {
    for (auto & m : messages) m = {};
  }

  export void push(jute::heap msg) {
    // TODO: merge duplicate messages (ex: "You got healed x3")
    for (auto i = 0; i < messages.size() - 1; i++) {
      messages[i] = messages[i + 1];
    }
    messages[messages.size() - 1] = message {
      .start = timer.secs(),
      .text = msg,
    };
  }

  export void for_each(auto && fn) {
    for (auto m : messages) fn((timer.secs() - m.start), *m.text);
  }
}
