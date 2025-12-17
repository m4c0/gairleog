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
    clay::nearest_texture txt { "pixelite2.png" };
    clay::vert_shader vert { "gairleog", [] {} };
    clay::frag_shader frag { "gairleog", [] {} };
    clay::buffer<sprite> buffer { 10240 };

    base_app_stuff() : vinyl::base_app_stuff("Gairleog") {}

    [[nodiscard]] operator bool() const { return txt && vert && frag; }

    using vtx_attrs_t = decltype(buffer)::vertex_attributes_t;
    static vtx_attrs_t vertex_attributes(); 
  };
}
