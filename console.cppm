export module console;
import casein;
import hai;
import imgui;
import jute;
import sprdef;

namespace console {
  hai::array<jute::heap> messages { 6 };

  export void reset() {
    for (auto & m : messages) m = {};
  }

  export void push(jute::heap msg) {
    for (auto i = 0; i < messages.size() - 1; i++) {
      messages[i] = messages[i + 1];
    }
    messages[messages.size() - 1] = msg;
  }

  export void draw(auto & m) {
    auto font = sprdef::get("font").id;

    m->set_grid({ 16, 16 });
    auto w = 16.f * casein::window_size.x / casein::window_size.y - 16.f;

    using namespace imgui;
    start(&*m, { -w + 0.5f, 31.5f - messages.size() }, [&] {
      vbox([&] {
        for (auto m : messages) {
          hbox([&] { text(font, m); });
        }
      });
    });
  }
}
