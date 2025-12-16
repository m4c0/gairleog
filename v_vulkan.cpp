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

struct app_stuff {
  voo::device_and_queue dq { "gairleog", casein::native_ptr };
  vee::render_pass rp = voo::single_att_render_pass(dq);
  vee::descriptor_set_layout dsl = vee::create_descriptor_set_layout({
    vee::dsl_fragment_sampler(),
  });
  vee::descriptor_pool dpool = vee::create_descriptor_pool(1, {
    vee::combined_image_sampler(1),
  });
  vee::pipeline_layout pl = vee::create_pipeline_layout(
      *dsl,
      vee::vertex_push_constant_range<float>());
  vee::gr_pipeline ppl = vee::create_graphics_pipeline({
    .pipeline_layout = *pl,
    .render_pass = *rp,
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    .back_face_cull = false,
    .shaders {
      *voo::vert_shader("gairleog.vert.spv"),
      *voo::frag_shader("gairleog.frag.spv"),
    },
    .bindings {
      vee::vertex_input_bind_per_instance(sizeof(v::sprite)),
    },
    .attributes {
      vee::vertex_attribute_vec2(0, traits::offset_of(&v::sprite::pos)),
      vee::vertex_attribute_vec2(0, traits::offset_of(&v::sprite::scale)),
      vee::vertex_attribute_vec4(0, traits::offset_of(&v::sprite::mult)),
      vee::vertex_attribute_vec2(0, traits::offset_of(&v::sprite::grid_pos)),
      vee::vertex_attribute_vec2(0, traits::offset_of(&v::sprite::grid_size)),
      vee::vertex_attribute_uint(0, traits::offset_of(&v::sprite::id)),
    },
  });
  voo::bound_buffer buf = voo::bound_buffer::create_from_host(
      dq.physical_device(),
      v::max_sprites * sizeof(v::sprite),
      vee::buffer_usage::vertex_buffer);

  vinyl::nearest_texture txt { "pixelite2.png" };

  unsigned count {};
};

struct ext_stuff {
  voo::swapchain_and_stuff sw { vv::as()->dq, *vv::as()->rp };
};

struct mapper : v::mapper, voo::memiter<v::sprite> {
  mapper() : voo::memiter<v::sprite> { *vv::as()->buf.memory, &vv::as()->count } {}
  virtual ~mapper() {}
  void add_sprite(v::sprite s) override { *this += s; }
};
hai::uptr<v::mapper> v::map() {
  return hai::uptr<v::mapper> { new ::mapper {} };
}

static void on_frame() try {
  if (!vv::as()->txt) return;

  vv::ss()->sw.acquire_next_image();
  vv::ss()->sw.queue_one_time_submit([] {
    v::call_on_frame();

    auto cb = vv::ss()->sw.command_buffer();
    auto ext = vv::ss()->sw.extent();

    // TODO: fix a rare sync issue between player and rest of game
    auto ofs = 0U;

    float pc = vv::ss()->sw.aspect();

    auto rp = vv::ss()->sw.cmd_render_pass({
      .clear_colours { vee::clear_colour(0, 0, 0, 1) },
    });
    vee::cmd_set_viewport(cb, ext);
    vee::cmd_set_scissor(cb, ext);
    vee::cmd_bind_gr_pipeline(cb, *vv::as()->ppl);
    vee::cmd_bind_descriptor_set(cb, *vv::as()->pl, 0, vv::as()->txt.dset);
    vee::cmd_push_vertex_constants(cb, *vv::as()->pl, &pc);
    vee::cmd_bind_vertex_buffers(cb, 0, *vv::as()->buf.buffer, ofs);
    vee::cmd_draw(cb, 4, vv::as()->count);
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
