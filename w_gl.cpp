module w;
import gelo;
import jute;
import silog;
import stubby;

using namespace jute::literals;

namespace w {
  static constexpr const float quad[] { 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0 };
  static constexpr const auto vert_shader = R"(#version 300 es
struct upc
{
    vec2 grid_pos;
    vec2 grid_size;
};

uniform upc _18;

layout(location = 0) in vec2 v_pos;
layout(location = 1) in vec2 pos;
out vec2 f_uv;
layout(location = 2) in vec2 uv;

void main()
{
    vec2 p = ((v_pos + pos) - _18.grid_pos) / _18.grid_size;
    gl_Position = vec4(p, 0.0, 1.0);
    f_uv = uv + v_pos;
}
  )"_s;
  static constexpr const auto frag_shader = R"(#version 300 es
precision mediump float;
precision highp int;

uniform highp sampler2D tex;

layout(location = 0) out highp vec4 colour;
in highp vec2 f_uv;

void main()
{
    colour = texelFetch(tex, ivec2(f_uv * 16.0), 0);
}
  )"_s;

  static void shader(int prog, int type, jute::view src) {
    using namespace gelo;
  
    auto v = create_shader(type);
    shader_source(v, src.begin(), src.size());
    compile_shader(v);
    if (!get_shader_parameter_b(v, COMPILE_STATUS)) {
      char buf[1024] {};
      get_shader_info_log(v, buf, sizeof(buf) - 1);
      silog::log(silog::error, "Error compiling shader:\n%s", buf);
    }
    attach_shader(prog, v);
  }

  auto setup() {
    using namespace gelo;

    struct {
      int program;
      int quad_buffer;
      int u_grid_pos;
      int u_grid_size;
    } res;

    auto p = res.program = create_program();
    shader(p, VERTEX_SHADER, vert_shader);
    shader(p, FRAGMENT_SHADER, frag_shader);

    link_program(p);
    if (!get_program_parameter_b(p, LINK_STATUS)) {
      char buf[1024] {};
      get_program_info_log(p, buf, sizeof(buf) - 1);
      silog::log(silog::error, "Error linking program:\n%s", buf);
    }

    use_program(p);

    res.u_grid_pos = get_uniform_location(p, "pc.grid_pos");
    res.u_grid_size = get_uniform_location(p, "pc.grid_size");
    auto u_tex = get_uniform_location(p, "tex");
    uniform1i(u_tex, 0); 

    auto b = res.quad_buffer = create_buffer();
    bind_buffer(ARRAY_BUFFER, b);
    buffer_data(ARRAY_BUFFER, quad, sizeof(quad), STATIC_DRAW);
    enable_vertex_attrib_array(0);
    vertex_attrib_pointer(0, 2, FLOAT, false, 0, 0);

    enable(BLEND);
    blend_func(ONE, ONE_MINUS_SRC_ALPHA);

    return res;
  }

  auto clear() {
    using namespace gelo;
    gelo::clear(COLOR_BUFFER_BIT);
    viewport(0, 0, casein::window_size.x, casein::window_size.y);
  }

  auto create_buffer() {
    static constexpr const auto stride = 80;

    using namespace gelo;

    auto b = gelo::create_buffer();
    bind_buffer(ARRAY_BUFFER, b);

    enable_vertex_attrib_array(1);
    vertex_attrib_pointer(1, 4, FLOAT, false, stride, 0);
    vertex_attrib_divisor(1, 1);

    enable_vertex_attrib_array(2);
    vertex_attrib_pointer(2, 4, FLOAT, false, stride, 32);
    vertex_attrib_divisor(2, 1);

    enable_vertex_attrib_array(3);
    vertex_attrib_pointer(3, 4, FLOAT, false, stride, 16);
    vertex_attrib_divisor(3, 1);

    enable_vertex_attrib_array(4);
    vertex_attrib_pointer(4, 4, FLOAT, false, stride, 48);
    vertex_attrib_divisor(4, 1);

    enable_vertex_attrib_array(5);
    vertex_attrib_pointer(5, 4, FLOAT, false, stride, 64);
    vertex_attrib_divisor(5, 1);

    return b;
  }

  auto create_texture() {
    using namespace gelo;

    unsigned pix = 0xFF00FFFF;

    auto t = gelo::create_texture();
    active_texture(TEXTURE0);
    bind_texture(TEXTURE_2D, t);
    tex_image_2d(TEXTURE_2D, 0, RGBA, 1, 1, 0, RGBA, UNSIGNED_BYTE, &pix, sizeof(pix));
    return t;
  }

  void load_texture(int t, jute::view name) {
    stbi::load_from_resource(name, [=](auto & img) {
      using namespace gelo;

      silog::log(silog::info, "[%*s] loaded", static_cast<int>(name.size()), name.begin());

      auto & [ w, h, ch, data ] = img;
      bind_texture(TEXTURE_2D, t);
      tex_image_2d(TEXTURE_2D, 0, RGBA, w, h, 0, RGBA, UNSIGNED_BYTE, *data, w * h * 4);
      tex_parameter_i(TEXTURE_2D, TEXTURE_WRAP_S, CLAMP_TO_EDGE);
      tex_parameter_i(TEXTURE_2D, TEXTURE_WRAP_T, CLAMP_TO_EDGE);
      tex_parameter_i(TEXTURE_2D, TEXTURE_MIN_FILTER, NEAREST);
      tex_parameter_i(TEXTURE_2D, TEXTURE_MAG_FILTER, NEAREST);
    });
  }
}

