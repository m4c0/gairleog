#version 450

layout(push_constant) uniform upc {
  vec2 grid_pos;
  vec2 grid_size;
};

layout(location = 0) in vec2 v_pos;
layout(location = 1) in vec2 pos;
layout(location = 2) in uint id;

layout(location = 0) out vec2 f_uv;

void main() {
  vec2 p = (v_pos + pos - grid_pos) / grid_size;

  gl_Position = vec4(p, 0, 1);
  f_uv = v_pos + ivec2(id % 64, id / 64);
}
