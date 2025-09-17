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

  g_map.build();

  auto m = v::map();
  g_map.load(m);
  g_map.at(g_pos) = 0;
  m->push({ .pos = g_pos, .id = sprdef::get("characters/human_knight") });
}

const int i = [] {
  res::on_error([](auto msg) { silog::die("Failure loading resource: %s", msg.cstr().begin()); });
  res::load_all([] {
    v::pc = { 16, 16 };
    v::on_frame = on_frame;
  });
  return 0;
}();
