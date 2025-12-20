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
  auto m = v::map();
  m->set_grid({ 0, 6 });
  fx::draw(m);
}

static void on_res_loaded() {
  using namespace casein;
  v::on<KEY_DOWN, K_SPACE, [] {
    float x = rng::rand(6) - 3.f;
    float y = rng::rand(6) - 3.f;
    fx::add({x, y}, sprdef::get("fx/fx_blue_bite"));
  }>();

  v::on_frame(on_frame);
}

const int i = [] {
  rng::seed();
  v::push<[] {
    res::load_all(on_res_loaded);
  }>();
  return 0;
}();
