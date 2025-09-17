#pragma leco app

import casein;
import map;
import res;
import silog;
import sprdef;
import v;

static map g_map {};

static void on_frame() {
  v::on_frame = [] {};

  g_map.build();

  auto m = v::map();
  g_map.load(m);
  m->push({ .pos {}, .id = sprdef::get("characters/human_knight") });
}

const int i = [] {
  res::on_error([](auto msg) { silog::die("Failure loading resource: %s", msg.cstr().begin()); });
  res::load_all([] {
    v::pc = { 16, 16 };
    v::on_frame = on_frame;
  });
  return 0;
}();
