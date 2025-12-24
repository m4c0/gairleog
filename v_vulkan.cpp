module v;
import :tools;
import casein;
import dotz;
import hai;
import lispy;
import silog;
import sires;
import traits;
import vee;
import vinyl;
import voo;
import wagen;

struct app_stuff : v::base_app_stuff {
};

struct ext_stuff {
  vee::render_pass rp = voo::single_att_render_pass(vv::as()->dq);
  voo::swapchain_and_stuff sw { vv::as()->dq, *rp };
};

struct mapper : v::mapper {
  decltype(vv::as()->ppl.map()) m = vv::as()->ppl.map();

  void add_sprite(v::sprite s) override { m += s; }
};
hai::uptr<v::mapper> v::map() {
  return hai::uptr<v::mapper> { new ::mapper {} };
}

// TODO: fix a rare sync issue between player and rest of game
static void on_frame() try {
  vv::ss()->sw.acquire_next_image();
  vv::ss()->sw.queue_one_time_submit([] {
    v::call_on_frame();

    auto cb = vv::ss()->sw.command_buffer();
    auto ext = vv::ss()->sw.extent();

    float pc = vv::ss()->sw.aspect();

    auto rp = vv::ss()->sw.cmd_render_pass({
      .clear_colours { vee::clear_colour(0, 0, 0, 1) },
    });
    vee::cmd_set_viewport(cb, ext);
    vee::cmd_set_scissor(cb, ext);
    vv::as()->ppl.cmd_draw(cb, &pc);
  });
  vv::ss()->sw.queue_present();
} catch (const lispy::parser_error & e) {
  silog::die("%s", lispy::to_file_err(e).begin());
} catch (const hai::cstr & msg) {
  silog::die("Error: %s", msg.begin());
}

const int i = [] {
  vv::setup(on_frame);
  return 0;
}();
