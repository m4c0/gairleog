#pragma leco app

import casein;
import dotz;
import hai;
import mtx;
import res;
import rng;
import silog;
import sitime;
import sprdef;
import sv;
import v;

namespace fx {
  struct t {
    sprdef::pair spr {};
    sitime::stopwatch timer {};
    dotz::vec2 pos {};
  };

  static hai::varray<t> data { 16 };
  static mtx::mutex mutex {};

  void add(dotz::vec2 p, sv sprite) {
    mtx::lock l { &mutex };

    data.push_back(t {
      .spr = sprdef::get(sprite),
      .timer = {},
      .pos = p,
    });
  }

  void draw(auto & m) {
    mtx::lock l { &mutex };

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
    rng::seed();

    res::load_all([] {
      using namespace casein;
      handle(KEY_DOWN, K_SPACE, [] {
        float x = rng::rand(6) - 3.f;
        float y = rng::rand(6) - 3.f;
        fx::add({x, y}, "fx/fx_blue_bite");
      });

      v::on_frame = on_frame;
    });
  } catch (const hai::cstr & e) {
    silog::die("Failure loading resource: %s", e.begin());
    throw;
  }
  return 0;
}();
