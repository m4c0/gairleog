#pragma leco app
#pragma leco add_resource_dir sprites
#pragma leco add_shader gairleog.frag
#pragma leco add_shader gairleog.vert

import casein;
import dotz;
import traits;
import vinyl;
import voo;

struct sprite {
  dotz::vec2 pos;
  dotz::vec2 uv;
};

struct upc {
  dotz::vec2 grid_pos;
  dotz::vec2 grid_size;
} g_pc;

struct app_stuff {
  voo::device_and_queue dq { "gairleog", casein::native_ptr };
  vee::render_pass rp = voo::single_att_render_pass(dq);
  voo::single_frag_dset dset { 1 };
  vee::pipeline_layout pl = vee::create_pipeline_layout(
      dset.descriptor_set_layout(),
      vee::vertex_push_constant_range<upc>());
  vee::gr_pipeline ppl = vee::create_graphics_pipeline({
    .pipeline_layout = *pl,
    .render_pass = *rp,
    .shaders {
      voo::shader("gairleog.vert.spv").pipeline_vert_stage(),
      voo::shader("gairleog.frag.spv").pipeline_frag_stage(),
    },
    .bindings {
      voo::one_quad::vertex_input_bind(),
      vee::vertex_input_bind_per_instance(sizeof(sprite)),
    },
    .attributes {
      voo::one_quad::vertex_attribute(0),
      vee::vertex_attribute_vec2(1, traits::offset_of(&sprite::pos)),
      vee::vertex_attribute_vec2(1, traits::offset_of(&sprite::uv)),
    },
  });
  voo::one_quad oqr { dq };
} * g_as;

struct ext_stuff {
  voo::swapchain_and_stuff sw { g_as->dq, *g_as->rp };
} * g_es;

const int i = [] {
  using namespace vinyl;
  on(START,  [] { g_as = new app_stuff {}; });
  on(RESIZE, [] { g_es = new ext_stuff {}; });
  on(FRAME,  [] {
    auto q = g_as->dq.queue();

    g_es->sw.acquire_next_image();
    g_es->sw.queue_one_time_submit(q, [] {
      auto cb = g_es->sw.command_buffer();
      auto ext = g_es->sw.extent();

      auto rp = g_es->sw.cmd_render_pass();
      vee::cmd_set_viewport(cb, ext);
      vee::cmd_set_scissor(cb, ext);
      vee::cmd_bind_gr_pipeline(cb, *g_as->ppl);
      vee::cmd_bind_descriptor_set(cb, *g_as->pl, 0, g_as->dset.descriptor_set());
      vee::cmd_push_vertex_constants(cb, *g_as->pl, &g_pc);
      g_as->oqr.run(cb, 1);
    });
    g_es->sw.queue_present(q);
  });
  on(STOP,   [] {
    delete g_es;
    delete g_as;
  });
  return 0;
}();
