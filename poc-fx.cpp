#pragma leco app

import casein;
import fx;
import hai;
import res;
import rng;
import silog;
import v;

static void on_frame() {
  v::set_grid({ 0, 6 });

  auto m = v::map();
  fx::draw(m);
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
