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

  export void reset() {
    mtx::lock l { &mutex };
    data.truncate(0);
  }

  export void add(dotz::vec2 p, sprdef::pair sprite) {
    if (!sprite.id) return;

    mtx::lock l { &mutex };

    data.push_back(t {
      .spr = sprite,
      .timer = {},
      .pos = p,
    });
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

