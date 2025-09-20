module v;
import casein;
import dotz;
import hai;
import sires;
import traits;
import vee;
import vinyl;
import voo;
import wagen;

struct app_stuff {
  voo::device_and_queue dq { "gairleog", casein::native_ptr };
  vee::render_pass rp = voo::single_att_render_pass(dq);
  voo::single_frag_dset dset { 1 };
  vee::pipeline_layout pl = vee::create_pipeline_layout(
      dset.descriptor_set_layout(),
      vee::vertex_push_constant_range<v::upc>());
  vee::gr_pipeline ppl = vee::create_graphics_pipeline({
    .pipeline_layout = *pl,
    .render_pass = *rp,
    .shaders {
      voo::shader("gairleog.vert.spv").pipeline_vert_stage(),
      voo::shader("gairleog.frag.spv").pipeline_frag_stage(),
    },
    .bindings {
      voo::one_quad::vertex_input_bind(),
      vee::vertex_input_bind_per_instance(sizeof(v::sprite)),
    },
    .attributes {
      voo::one_quad::vertex_attribute(0),
      vee::vertex_attribute_vec2(1, traits::offset_of(&v::sprite::pos)),
      vee::vertex_attribute_uint(1, traits::offset_of(&v::sprite::id)),
      vee::vertex_attribute_float(1, traits::offset_of(&v::sprite::mult)),
    },
  });
  vee::sampler smp = [] {
    auto info = vee::sampler_create_info {};
    info.address_mode(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
    info.nearest();
    info.unnormalizedCoordinates = wagen::vk_true;
    return vee::create_sampler(info);
  }();
  voo::one_quad oq { dq };
  voo::bound_buffer buf = voo::bound_buffer::create_from_host(
      dq.physical_device(),
      v::max_sprites * sizeof(v::sprite),
      vee::buffer_usage::vertex_buffer);
  voo::bound_image img {};
  unsigned count {};
} * g_as;

struct ext_stuff {
  voo::swapchain_and_stuff sw { g_as->dq, *g_as->rp };
} * g_es;

struct mapper : v::mapper, voo::memiter<v::sprite> {
  mapper() : voo::memiter<v::sprite> { *g_as->buf.memory, &g_as->count } {}
  virtual ~mapper() {}
  void push(v::sprite s) override { *this += s; }
};
hai::uptr<v::mapper> v::map() {
  return hai::uptr<v::mapper> { new ::mapper {} };
}

const int i = [] {
  using namespace vinyl;
  on(START,  [] {
    g_as = new app_stuff {};

    voo::load_image("pixelite2.png", g_as->dq.physical_device(), g_as->dq.queue(), &g_as->img, [] {
      vee::update_descriptor_set(g_as->dset.descriptor_set(), 0, i, *g_as->img.iv, *g_as->smp);
    });
  });
  on(RESIZE, [] { delete g_es; g_es = nullptr; });
  on(FRAME,  [] {
    if (!g_es) g_es = new ext_stuff {};

    auto q = g_as->dq.queue();

    g_es->sw.acquire_next_image();
    g_es->sw.queue_one_time_submit(q, [] {
      v::on_frame();

      auto cb = g_es->sw.command_buffer();
      auto ext = g_es->sw.extent();

      auto pc = v::pc;
      pc.grid_size.x *= g_es->sw.aspect();

      auto rp = g_es->sw.cmd_render_pass({
        .clear_colours { vee::clear_colour(0, 0, 0, 1) },
      });
      vee::cmd_set_viewport(cb, ext);
      vee::cmd_set_scissor(cb, ext);
      vee::cmd_bind_gr_pipeline(cb, *g_as->ppl);
      vee::cmd_bind_descriptor_set(cb, *g_as->pl, 0, g_as->dset.descriptor_set());
      vee::cmd_push_vertex_constants(cb, *g_as->pl, &pc);
      vee::cmd_bind_vertex_buffers(cb, 1, *g_as->buf.buffer);
      g_as->oq.run(cb, 0, g_as->count);
    });
    g_es->sw.queue_present(q);
  });
  on(STOP,   [] {
    delete g_es;
    delete g_as;
  });

  return 0;
}();
