#pragma leco app

import dotz;
import errs;
import hai;
import imgui;
import lispy;
import res;
import silog;
import sprdef;
import sv;
import v;

static void on_frame() {
  v::set_grid({ 6, 6 });

  auto font = sprdef::get("font").id;

  auto brown  = sprdef::get("fx/fx_brown_bite" ).id + 4;
  auto blue   = sprdef::get("fx/fx_blue_bite"  ).id + 4;
  auto purple = sprdef::get("fx/fx_purple_bite").id + 4;
  auto gray   = sprdef::get("fx/fx_gray_bite"  ).id + 4;
  auto orange = sprdef::get("fx/fx_orange_bite").id + 4;

  auto m = v::map();

  using namespace imgui;
  start(&*m, {}, [&] {
    vbox([&] {
      hbox([&] {
        sprite(brown);
        sprite(blue);
        sprite(purple);
      });
      scale({ 0.5f }, [&] {
        hbox([&] {
          sprite(gray);
          space({ 0.5f });
          text(font, "ok!");
        });
      });
      hbox([&] {
        sprite(brown);
        sprite(blue);
        sprite(purple);
        sprite(orange);
        sprite(gray);
      });
    });
  });
}

const auto i = [] {
  try {
    res::load_all([] {
      v::on_frame = on_frame;
    });
  } catch (const lispy::parser_error & e) {
    silog::die("%s", lispy::to_file_err(e).begin());
  } catch (const hai::cstr & err) {
    silog::die("Failed to load resource: %s", err.begin());
  }
  return 0;
}();
