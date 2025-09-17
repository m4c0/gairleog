#pragma leco app

import casein;
import dotz;
import map;
import res;
import silog;
import sprdef;
import v;

static map g_map {};

static dotz::ivec2 g_pos { 1 };

static void on_frame() {
  v::on_frame = [] {};

  auto m = v::map();
  g_map.load(m);
  m->push({ .pos = g_pos, .id = sprdef::get("characters/human_knight") });
}

static constexpr const auto move(int dx, int dy) {
  return [=] {
    auto p = g_pos + dotz::ivec2 { dx, dy };
    if (g_map.at(p)) return;

    g_pos = p;
    v::on_frame = on_frame;
  };
}

const int i = [] {
  res::on_error([](auto msg) { silog::die("Failure loading resource: %s", msg.cstr().begin()); });
  res::load_all([] {
    v::pc = { 16, 16 };
    v::on_frame = on_frame;
    g_map.build();
    g_map.at(g_pos) = 0;
  });

  using namespace casein;
  handle(KEY_DOWN, K_UP,    move(0, -1));
  handle(KEY_DOWN, K_DOWN,  move(0, +1));
  handle(KEY_DOWN, K_LEFT,  move(-1, 0));
  handle(KEY_DOWN, K_RIGHT, move(+1, 0));

  return 0;
}();
