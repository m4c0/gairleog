#version 450

layout(set = 0, binding = 0) uniform sampler2D tex;

layout(location = 0) in vec2 f_uv;

layout(location = 0) out vec4 colour;

void main() {
  colour = texture(tex, f_uv);
}
