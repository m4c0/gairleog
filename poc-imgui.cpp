#pragma leco app

import dotz;
import hai;
import res;
import silog;
import sprdef;
import v;

static void on_frame() {
  v::set_grid({ 6, 6 });

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
  const auto hbox = [&](auto && fn) {
    auto old_delta = s.delta;
    auto old_x = s.pos.x;

    s.delta = { 1, 0 };
    fn();
    
    s.delta = old_delta;
    s.pos.x = old_x;;
    s.pos = s.pos + s.delta;
  };
  const auto vbox = [&](auto && fn) {
    auto old_delta = s.delta;
    auto old_y = s.pos.y;

    s.delta = { 0, 1 };
    fn();

    s.delta = old_delta;
    s.pos.y = old_y;
    s.pos = s.pos + s.delta;
  };
  const auto sprite = [&](auto id) {
    m->push({
      .pos = s.pos,
      .id = id,
    });
    s.pos = s.pos + s.delta;
  };

  vbox([&] {
    hbox([&] {
      sprite(brown);
      sprite(blue);
      sprite(purple);
    });
    hbox([&] {
      sprite(gray);
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
