#version 450

layout(set = 0, binding = 0) uniform sampler2D tex;

layout(location = 0) in vec2 f_uv;
layout(location = 1) in vec4 f_mult;

layout(location = 0) out vec4 colour;

void main() {
  vec4 c = texelFetch(tex, ivec2(f_uv * 16), 0);
  colour = c * f_mult;
}
