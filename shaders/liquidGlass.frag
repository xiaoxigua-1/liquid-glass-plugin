#version 300 es
precision highp float;

uniform vec2 fullSize;
uniform float radius;

uniform sampler2D tex;

in vec2 v_texcoord;

layout(location = 0) out vec4 fragColor;

void main()
{
  vec2 uv = v_texcoord;
  vec2 center = vec2(0.5, 0.5);
  vec2 fromCenter = uv - center;
  float dist = length(fromCenter);

  // 擴大折射範圍和強度
  vec2 refraction = fromCenter * sin(dist * 3.14159 * 1.5) * 0.08; // 增強折射
  vec2 lensUV = uv + refraction;

  // 確保採樣不超出邊界
  lensUV = clamp(lensUV, 0.0, 1.0);

  // 直接採樣
  vec3 color = texture(tex, lensUV).rgb;

  // 調整光澤效果 - 降低強度，並且只在邊緣出現
  float fresnel = pow(1.0 - dist * 2.0, 3.0); // 提高次方，讓光暈更集中在邊緣
  fresnel = clamp(fresnel, 0.0, 1.0);

  fragColor = vec4(color, 1.0);
}
