#pragma leco app

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
  m->push({
    .pos { 0, 0 },
    .id = brown,
  });
  m->push({
    .pos { 1, 0 },
    .id = blue,
  });
  m->push({
    .pos { 2, 0 },
    .id = purple,
  });
  m->push({
    .pos { 0, 1 },
    .id = gray,
  });
  m->push({
    .pos { 0, 2 },
    .id = orange,
  });
  m->push({
    .pos { 1, 2 },
    .id = blue,
  });
  m->push({
    .pos { 2, 2 },
    .id = purple,
  });
  m->push({
    .pos { 3, 2 },
    .id = orange,
  });
  m->push({
    .pos { 4, 2 },
    .id = brown,
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
