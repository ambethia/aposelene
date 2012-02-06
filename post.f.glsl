uniform sampler2D fbo_texture;
varying vec2 f_texcoord;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void) {
  float gray = rand(f_texcoord);
  gl_FragColor = mix(
    texture2D(fbo_texture, f_texcoord),
    vec4(gray,gray,gray,0.5),
    0.1
  );
}
