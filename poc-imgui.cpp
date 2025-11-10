#pragma leco app

import dotz;
import hai;
import res;
import silog;
import sprdef;
import sv;
import v;

static void on_frame() {
  v::set_grid({ 6, 6 });

  auto font = sprdef::get("font").id;

  auto brown = sprdef::get("fx/fx_brown_bite").id + 4;
  auto blue = sprdef::get("fx/fx_blue_bite").id + 4;
  auto purple = sprdef::get("fx/fx_purple_bite").id + 4;
  auto gray = sprdef::get("fx/fx_gray_bite").id + 4;
  auto orange = sprdef::get("fx/fx_orange_bite").id + 4;

  auto m = v::map();
  struct state {
    dotz::vec2 pos {};
    dotz::vec2 delta {};
  } s;
  const auto box = [&](dotz::vec2 delta, auto && fn) {
    auto old_delta = s.delta;
    auto old_pos = s.pos;

    s.delta = delta;
    fn();
    
    s.delta = old_delta;
    s.pos = old_pos + s.delta;
  };
  const auto hbox = [&](auto && fn) {
    box({ 1, 0 }, fn);
  };
  const auto vbox = [&](auto && fn) {
    box({ 0, 1 }, fn);
  };
  const auto sprite = [&](auto id) {
    box({}, [&] {
      m->push({
        .pos = s.pos,
        .id = id,
      });
    });
  };
  const auto text = [&](sv str) {
    for (auto c : str) {
      sprite(font + c);
    }
  };

  vbox([&] {
    hbox([&] {
      sprite(brown);
      sprite(blue);
      sprite(purple);
    });
    hbox([&] {
      sprite(gray);
      text("ok!");
    });
    hbox([&] {
      sprite(brown);
      sprite(blue);
      sprite(purple);
      sprite(orange);
      sprite(gray);
    });
  });
}

const auto i = [] {
  try {
    res::load_all([] {
      v::on_frame = on_frame;
    });
  } catch (const hai::cstr & err) {
    silog::die("Failed to load resource: %s", err.begin());
  }
  return 0;
}();
