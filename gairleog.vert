#version 450

layout(push_constant) uniform upc {
  vec2 grid_pos;
  vec2 grid_size;
};

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 i_pos;
layout(location = 2) in vec2 i_uv;

void main() {
  gl_Position = vec4(pos, 0, 1);
}
