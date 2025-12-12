export module console;
import casein;
import dotz;
import hai;
import imgui;
import jute;
import sitime;
import sprdef;

namespace console {
  struct message {
    float start {};
    jute::heap text {};
  };
  hai::array<message> messages { 6 };

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

  export void draw(auto & m) {
    auto font = sprdef::get("font").id;

    m->set_grid({ 16, 16 });
    auto w = 16.f * casein::window_size.x / casein::window_size.y - 16.f;

    using namespace imgui;
    start(&*m, { -w + 0.5f, 31.5f - messages.size() }, [&] {
      vbox([&] {
        for (auto m : messages) {
          float a = 1.0f - dotz::clamp((timer.secs() - m.start) * 0.2f, 0.0f, 1.0f);
          mult(a, [&] {
            hbox([&] { text(font, m.text); });
          });
        }
      });
    });
  }
}
