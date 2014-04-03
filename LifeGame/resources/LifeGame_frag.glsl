uniform vec2 pixel;
uniform sampler2D src;

varying vec2 uv;

void main() {
  int count = 0;

  count += texture2D(src, vec2(uv.x - pixel.x, uv.y - pixel.y));
  count += texture2D(src, vec2(uv.x - pixel.x, uv.y));
  count += texture2D(src, vec2(uv.x - pixel.x, uv.y + pixel.y));

  count += texture2D(src, vec2(uv.x, uv.y - pixel.y));
  count += texture2D(src, vec2(uv.x, uv.y + pixel.y));

  count += texture2D(src, vec2(uv.x + pixel.x, uv.y - pixel.y));
  count += texture2D(src, vec2(uv.x + pixel.x, uv.y));
  count += texture2D(src, vec2(uv.x + pixel.x, uv.y + pixel.y));

  if (count == 3) {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  } else if (count == 2) {
    gl_FragColor = texture2D(src, uv);
  } else {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}
