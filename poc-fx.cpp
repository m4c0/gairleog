#pragma leco app

import casein;
import dotz;
import hai;
import sprdef;
import res;
import silog;
import sitime;
import v;

static sprdef::pair spr;
static sitime::stopwatch timer;
static dotz::vec2 pos;

static void on_frame() try {
  v::set_grid({ 0, 6 });

  auto m = v::map();

  if (spr.id == 0) return;

  auto frame = timer.millis() / 50;
  if (frame >= spr.qty) {
    spr = {};
    return;
  }

  m->push({
    .pos = pos,
    .id = spr.id + frame,
  });
} catch (const hai::cstr & err) {
  silog::die("%s", err.begin());
}

const int i = [] {
  try {
    res::load_all([] {
      using namespace casein;
      handle(KEY_DOWN, K_SPACE, [] {
        pos = {};
        spr = sprdef::get("fx/fx_blue_bite");
        timer = {};
      });

      v::on_frame = on_frame;
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
    throw;
  }
  return 0;
}();
