#version 450

layout(push_constant) uniform upc {
  float aspect;
} pc;

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 scale;
layout(location = 2) in vec4 mult;
layout(location = 3) in vec2 grid_pos;
layout(location = 4) in vec2 grid_size;
layout(location = 5) in float rotation;
layout(location = 6) in uint id;

layout(location = 0) out vec2 f_uv;
layout(location = 1) out vec4 f_mult;

void main() {
  float rad_rot = radians(rotation);

  vec2 v_pos = vec2(gl_VertexIndex & 1, (gl_VertexIndex >> 1) & 1);

  vec2 r_pos = v_pos - 0.5;
  r_pos = mat2(cos(rad_rot), sin(rad_rot), -sin(rad_rot), cos(rad_rot)) * r_pos;
  r_pos += 0.5;

  vec2 asp = vec2(pc.aspect, 1);
  vec2 p = (r_pos * scale + pos - grid_pos) / (grid_size * asp);

  gl_Position = vec4(p, 0, 1);
  f_uv = ivec2(id % 64, id / 64) + v_pos * 0.999 + 0.001;
  f_mult = mult;
}
