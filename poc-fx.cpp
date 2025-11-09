#pragma leco app

import casein;
import dotz;
import hai;
import sprdef;
import res;
import silog;
import sitime;
import sv;
import v;

namespace fx {
  struct t {
    sprdef::pair spr {};
    sitime::stopwatch timer {};
    dotz::vec2 pos {};
  };

  static hai::varray<t> data { 16 };

  void add(dotz::vec2 p, sv sprite) {
    data.push_back(t {
      .spr = sprdef::get(sprite),
      .pos = p,
    });
  }

  void draw(auto & m) {
    for (int i = data.size() - 1; i >= 0; i--) {
      auto & t = data[i];

      auto frame = t.timer.millis() / 50;
      if (!t.spr.id || frame >= t.spr.qty) {
        t = data.pop_back();
        continue;
      }

      m->push({
        .pos = t.pos,
        .id = t.spr.id + frame,
      });
    }
  }
}

static void on_frame() try {
  v::set_grid({ 0, 6 });

  auto m = v::map();
  fx::draw(m);
} catch (const hai::cstr & err) {
  silog::die("%s", err.begin());
}

const int i = [] {
  try {
    res::load_all([] {
      using namespace casein;
      handle(KEY_DOWN, K_SPACE, [] {
        fx::add({}, "fx/fx_blue_bite");
      });

      v::on_frame = on_frame;
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
    throw;
  }
  return 0;
}();
