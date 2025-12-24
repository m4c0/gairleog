#pragma leco add_impl v_windows_hack
export module v:tools;
import :objects;
import clay;
import hai;
import sv;
import vinyl;

struct app_stuff;
struct ext_stuff;
using vv = vinyl::v<app_stuff, ext_stuff>;

namespace v {
  struct base_app_stuff : vinyl::base_app_stuff {
    clay::das::pipeline<sprite> ppl {{
      .app = this,
      .shader = "gairleog",
      .texture = "pixelite2.png",
      .max_instances = 10240,
      .vertex_attributes = vertex_attributes(),
      .push_constant = clay::vertex_push_constant<float>(),
    }};

    base_app_stuff() : vinyl::base_app_stuff("Gairleog") {}

    using vtx_attrs_t = clay::vertex_attributes_t;
    static vtx_attrs_t vertex_attributes(); 
  };
}
