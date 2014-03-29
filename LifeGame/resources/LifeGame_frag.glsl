uniform vec2 pixel;
uniform sampler2D src;

varying vec2 uv;

vec2 offsets[8];
float hinc;
float vinc;

void main() {
  vinc = pixel.x;
  hinc = pixel.y;
  offsets[0] = vec2(-vinc, -hinc);
  offsets[1] = vec2(-vinc, 0.0);
  offsets[2] = vec2(-vinc, hinc);
  offsets[3] = vec2(0.0, -hinc);
  offsets[4] = vec2(0.0, hinc);
  offsets[5] = vec2(vinc, -hinc);
  offsets[6] = vec2(vinc, 0.0);
  offsets[7] = vec2(vinc, hinc);

  vec4 current = texture2D(src, uv);

  int count = 0;
  for (int i = 0; i < 8; ++i) {
    vec2 neighbor = vec2(uv.x + offsets[i].x, uv.y + offsets[i].y);
    if (texture2D(src, neighbor).r > 0.9) {
      count++;
    }
  }

  if (count == 3) {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  } else if (count == 2) {
    gl_FragColor = current;
  } else {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}
