export module console;
import casein;
import imgui;
import sprdef;

namespace console {
  export void reset() {}

  export void draw(auto & m) {
    auto font = sprdef::get("font").id;

    m->set_grid({ 16, 16 });
    auto w = 16.f * casein::window_size.x / casein::window_size.y - 16.f;

    using namespace imgui;
    start(&*m, { -w + 0.5f, 30.5f }, [&] {
      hbox([&] {
        //text(font, "You gained strength");
      });
    });
  }
}
