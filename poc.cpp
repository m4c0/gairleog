#pragma leco app
#pragma leco add_resource_dir sprites

import casein;
import vinyl;
import voo;

struct app_stuff {
  voo::device_and_queue dq { "poc-vinyl", casein::native_ptr };
  vee::render_pass rp = voo::single_att_render_pass(dq);
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
    });
    g_es->sw.queue_present(q);
  });
  on(STOP,   [] {
    delete g_es;
    delete g_as;
  });
  return 0;
}();
