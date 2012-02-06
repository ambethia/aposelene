uniform sampler2D fbo_texture;
uniform vec4 screen_sizes;
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
  vec4 bloom;
  for(int i= -4 ;i < 4; i++) {
    for (int j = -3; j < 3; j++) {
      sum += texture2D(fbo_texture, f_texcoord + vec2(j, i)*0.004) * 0.25;
    }
  }
  if (noise.r < 0.3) {
    bloom = sum*sum*0.012 + noise;
  } else {
    if (noise.r < 0.5) {
      bloom = sum*sum*0.009 + noise;
    } else {
      bloom = sum*sum*0.0075 + noise;
    }
  }

  // The two screen sizes are packed into one vec4
  vec2 faux = screen_sizes.xy;
  vec2 real = screen_sizes.zw;
  vec2 scale = real / faux;

  // We want to flip on x, so the shading appears lit from the top left
  vec2 coords = vec2(1.0 - f_texcoord.x, f_texcoord.y);

  // Now figure out the shading for each superpixel
  vec2 step = mod(coords * real, scale);
  float value = (step.x + step.y) / (scale.x + scale.y);

  // Normalize the range from 0->1 to 0.5->1 so black isn't so black.
  vec4 gradient = vec4(vec3((value / 2.0) + 0.5), 1.0);

  gl_FragColor = bloom * gradient;
}
