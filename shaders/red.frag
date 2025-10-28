#version 300 es

precision highp float;

uniform sampler2D tex;

in vec2 v_texcoord;

layout(location = 0) out vec4 fragColor;

void main() {
  vec4 color = texture(tex, v_texcoord);
  color.r = min(color.r * 1.5, 1.0);
  fragColor = color;
}
