#pragma leco app

import hai;
import sprdef;
import res;
import silog;
import sitime;
import v;

static sprdef::pair spr;
static sitime::stopwatch timer;

static void on_frame() try {
  v::set_grid({ 0, 6 });

  auto frame = timer.millis() / 50;

  auto m = v::map();
  m->push({
    .pos = { 0 },
    .id = spr.id + frame % spr.qty,
  });
} catch (const hai::cstr & err) {
  silog::die("%s", err.begin());
}

const int i = [] {
  try {
    res::load_all([] {
      spr = sprdef::get("fx/fx_blue_bite");
      timer = {};

      v::on_frame = on_frame;
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
    throw;
  }
  return 0;
}();
