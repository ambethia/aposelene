//
//  post_fragment.glsl
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

uniform sampler2D fboTexture;
uniform vec4 screenSize;
varying vec2 fragmentTexCoord;

float pseudoRand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 squeeze(float value, float min) {
  return vec4(vec3((value * min) + (1.0 - min)), 1.0);
}

void main(void) {
  vec4 color = texture2D(fboTexture, fragmentTexCoord);

  /*
   * BLOOM FILTER
   */
  
  vec4 sum = vec4(0);
  vec4 bloom;
  for(int i= -4 ;i < 4; i++) {
    for (int j = -3; j < 3; j++) {
      sum += texture2D(fboTexture, fragmentTexCoord + vec2(j, i)*0.004) * 0.25;
    }
  }
  if (color.r < 0.3) {
    bloom = sum*sum*0.012 + color;
  } else {
    if (color.r < 0.5) {
      bloom = sum*sum*0.009 + color;
    } else {
      bloom = sum*sum*0.0075 + color;
    }
  }

  /*
   * GRADIENT
   */

  // The two screen sizes are packed into one vec4
  vec2 faux = screenSize.xy;
  vec2 clip = screenSize.zw;
  vec2 scale = clip / faux;

  // We want to flip on x, so the shading appears lit from the top left
  vec2 coords = vec2(1.0 - fragmentTexCoord.x, fragmentTexCoord.y);

  // Now figure out the shading for each superpixel
  vec2 step = mod(coords * clip, scale);
  vec4 gradient = squeeze((step.x + step.y) / (scale.x + scale.y), 0.3);

  /*
   * NOISE
   */
  
  vec4 noise = squeeze(pseudoRand(fragmentTexCoord), 0.1);

  /*
   * OUTPUT
   */

  gl_FragColor = bloom * gradient * noise;
}
