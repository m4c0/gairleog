#pragma leco app
#pragma leco add_resource_dir sprites

import casein;
import vinyl;
import voo;

struct app_stuff {
  voo::device_and_queue dq { "poc-vinyl", casein::native_ptr };
  vee::render_pass rp = voo::single_att_render_pass(dq);
} * g_as;

struct sized_stuff {
  voo::swapchain_and_stuff sw { g_as->dq, *g_as->rp };
} * g_ss;

const int i = [] {
  using namespace vinyl;
  on(START,  [] { g_as = new app_stuff {}; });
  on(RESIZE, [] { g_ss = new sized_stuff {}; });
  on(FRAME,  [] {});
  on(STOP,   [] {
    delete g_ss;
    delete g_as;
  });
  return 0;
}();
