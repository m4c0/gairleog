#version 450

layout(push_constant) uniform upc {
  float aspect;
};

layout(std140, set = 0, binding = 1) uniform uni {
  vec2 grid_pos;
  vec2 grid_size;
};

layout(location = 0) in vec2 v_pos;
layout(location = 1) in vec2 pos;
layout(location = 2) in uint id;
layout(location = 3) in float mult;

layout(location = 0) out vec2 f_uv;
layout(location = 1) out float f_mult;

void main() {
  vec2 asp = vec2(aspect, 1);
  vec2 p = (v_pos + pos - grid_pos) / (grid_size * asp);

  gl_Position = vec4(p, 0, 1);
  f_uv = v_pos + ivec2(id % 64, id / 64);
  f_mult = mult;
}
