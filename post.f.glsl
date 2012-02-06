uniform sampler2D fbo_texture;
varying vec2 f_texcoord;

float pseudoRand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void) {
  // Noise
  float gray = pseudoRand(f_texcoord);
  vec4 noise = mix(
    texture2D(fbo_texture, f_texcoord),
    vec4(gray,gray,gray,0.5),
    0.1
  );

  // Bloom
  vec4 sum = vec4(0);
  for(int i= -4 ;i < 4; i++) {
    for (int j = -3; j < 3; j++) {
      sum += texture2D(fbo_texture, f_texcoord + vec2(j, i)*0.004) * 0.25;
    }
  }
  if (noise.r < 0.3) {
    gl_FragColor = sum*sum*0.012 + noise;
  } else {
    if (noise.r < 0.5) {
      gl_FragColor = sum*sum*0.009 + noise;
    } else {
      gl_FragColor = sum*sum*0.0075 + noise;
    }
  }
}
