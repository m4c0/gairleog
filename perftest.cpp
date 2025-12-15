#pragma leco app
import ents;
import glispy;
import hai;
import map;
import res;
import save;
import silog;
import v;

static void on_frame() {
  save::current_stage = 1 + save::current_stage % 40;
  glispy::reset();
  ents::reset();
  map::build();

  auto m = v::map();
  m->set_grid({ 16, 16 });
  ents::foreach([&](const auto & d) {
    m->push({
      .pos = d.pos,
      .id = d.sprite,
    });
  });
}

const int i = [] {
  try {
    res::load_all([] {
      v::on_frame = on_frame;
    });
  } catch (const lispy::parser_error & err) {
    silog::die("%s", lispy::to_file_err(err).begin());
  } catch (const hai::cstr & e) {
    silog::die("%s", e.begin());
  }
  return 0;
}();
