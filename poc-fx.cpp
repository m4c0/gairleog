#pragma leco app

import casein;
import errs;
import fx;
import hai;
import lispy;
import res;
import rng;
import silog;
import sprdef;
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
        fx::add({x, y}, sprdef::get("fx/fx_blue_bite"));
      });

      v::on_frame = on_frame;
    });
  } catch (const lispy::parser_error & err) {
    silog::die("%s", lispy::to_file_err(err).begin());
  } catch (const hai::cstr & e) {
    silog::die("%s", e.begin());
  }
  return 0;
}();
