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
  vee::render_pass rp = voo::single_att_render_pass(dq);

  vee::pipeline_layout pl = vee::create_pipeline_layout(
      *txt.dsl,
      vee::vertex_push_constant_range<float>());
  vee::gr_pipeline ppl = vee::create_graphics_pipeline({
    .pipeline_layout = *pl,
    .render_pass = *rp,
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    .back_face_cull = false,
    .shaders { *vert, *frag },
    .bindings { buffer.vertex_input_bind() },
    .attributes = vertex_attributes(),
  });
};

struct ext_stuff {
  voo::swapchain_and_stuff sw { vv::as()->dq, *vv::as()->rp };
};

struct mapper : v::mapper {
  decltype(vv::as()->buffer.map()) m = vv::as()->buffer.map();

  void add_sprite(v::sprite s) override { m += s; }
};
hai::uptr<v::mapper> v::map() {
  return hai::uptr<v::mapper> { new ::mapper {} };
}

// TODO: fix a rare sync issue between player and rest of game
static void on_frame() try {
  if (!vv::as()->txt) return;

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
    vee::cmd_bind_gr_pipeline(cb, *vv::as()->ppl);
    vee::cmd_bind_descriptor_set(cb, *vv::as()->pl, 0, vv::as()->txt.dset);
    vee::cmd_push_vertex_constants(cb, *vv::as()->pl, &pc);
    vv::as()->buffer.bind(cb);
    vee::cmd_draw(cb, 4, vv::as()->buffer.count());
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
