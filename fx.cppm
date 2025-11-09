export module fx;
import dotz;
import hai;
import mtx;
import silog;
import sitime;
import sprdef;
import sv;

namespace fx {
  struct t {
    sprdef::pair spr {};
    sitime::stopwatch timer {};
    dotz::vec2 pos {};
  };

  hai::varray<t> data { 16 };
  mtx::mutex mutex {};

  export void add(dotz::vec2 p, sv sprite) try {
    mtx::lock l { &mutex };

    data.push_back(t {
      .spr = sprdef::get(sprite),
      .timer = {},
      .pos = p,
    });
  } catch (const hai::cstr & err) {
    silog::log(silog::error, "Missing effect in sprdefs: %s", err.begin());
  }

  export void draw(auto & m) {
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

