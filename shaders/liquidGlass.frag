#version 300 es

#define R     fullSize
#define PI    3.14159265
#define S     smoothstep
#define PX(a) (a)/R.y

precision highp float;

uniform vec2 fullSize;
uniform sampler2D tex;
uniform float radius;

in vec2 v_texcoord;

mat2 Rot(float a) {
  return mat2(cos(a), sin(-a), sin(a), cos(a));
}

float Box(vec2 p, vec2 b, float r) {
  vec2 d = abs(p) - b + vec2(r);

  return length(max(d, 0.0)) - r + min(max(d.x, d.y), 0.0);
}

vec4 LiquidGlass(sampler2D tex, vec2 uv, float direction, float quality, float size) {
  vec2 radius = size / R;
  vec4 color = texture(tex, uv);

  for (float d = 0.; d < PI; d += PI / direction) {
    for (float i = 1. / quality; i <= 1.; i += 1. / quality) {
      color += texture(tex, uv + vec2(cos(d), sin(d)) * radius * i);
    }
  }

  color /= quality * direction;
  return color;
}

vec3 Window(vec2 uv, vec2 size, float radius, float light) {
  float box = Box(uv, size, PX(radius));
  float boxShape = S(PX(1.), 0., box);
  float boxDisp = S(PX(65.), 0., box + PX(25.));
  float boxLight = boxShape * S(0., PX(30.), box + PX(light));

  return vec3(boxShape, boxDisp, boxLight);
}

layout(location = 0) out vec4 fragColor;
void main() {
  vec2 I = v_texcoord;

  vec2 boxSize = vec2(PX(R.x / 2.), PX(R.y / 2.));
  float brightness = .3;
  float lightWidth = 7.;
  float blur = 20.;

  vec2 uv = I / R;
  vec2 st = (I - .5 * R) / R.y;

  vec3 w = Window(st, boxSize, radius, lightWidth);
  vec2 uv2 = uv * (.5 + .5 * S(.0, 1., w.y));

  vec3 col = mix(texture(tex, uv).rgb * 1., brightness + LiquidGlass(tex, uv2, 10., 10., blur).rgb * .7, w.x);
  col += w.z * .9;

  fragColor = vec4(col, 1.0);
}
