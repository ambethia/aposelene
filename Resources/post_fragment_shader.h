//
//  post_fragment_shader.h
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#ifndef _post_fragment_shader_h
#define _post_fragment_shader_h

static const char *post_fragment_shader =
  "uniform sampler2D fboTexture;\n"
  "uniform vec4 screenSize;\n"
  "varying vec2 fragmentTexCoord;\n"
  "float pseudoRand(vec2 co){\n"
  "  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\n"
  "}\n"
  "vec4 squeeze(float value, float min) {\n"
  "  return vec4(vec3((value * min) + (1.0 - min)), 1.0);\n"
  "}\n"
  "void main(void) {\n"
  "  vec4 color = texture2D(fboTexture, fragmentTexCoord);\n"
  "  /*\n"
  "   * BLOOM FILTER\n"
  "   */\n"
  "  vec4 sum = vec4(0);\n"
  "  vec4 bloom;\n"
  "  for(int i= -4 ;i < 4; i++) {\n"
  "    for (int j = -3; j < 3; j++) {\n"
  "      sum += texture2D(fboTexture, fragmentTexCoord + vec2(j, i)*0.004) * 0.25;\n"
  "    }\n"
  "  }\n"
  "  if (color.r < 0.3) {\n"
  "    bloom = sum*sum*0.012 + color;\n"
  "  } else {\n"
  "    if (color.r < 0.5) {\n"
  "      bloom = sum*sum*0.009 + color;\n"
  "    } else {\n"
  "      bloom = sum*sum*0.0075 + color;\n"
  "    }\n"
  "  }\n"
  "  /*\n"
  "   * GRADIENT\n"
  "   */\n"
  "  // The two screen sizes are packed into one vec4\n"
  "  vec2 faux = screenSize.xy;\n"
  "  vec2 clip = screenSize.zw;\n"
  "  vec2 scale = clip / faux;\n"
  "  // We want to flip on x, so the shading appears lit from the top left\n"
  "  vec2 coords = vec2(1.0 - fragmentTexCoord.x, fragmentTexCoord.y);\n"
  "  // Now figure out the shading for each superpixel\n"
  "  vec2 step = mod(coords * clip, scale);\n"
  "  vec4 gradient = squeeze((step.x + step.y) / (scale.x + scale.y), 0.33);\n"
  "  /*\n"
  "   * NOISE\n"
  "   */\n"
  "  vec4 noise = squeeze(pseudoRand(fragmentTexCoord), 0.1);\n"
  "  /*\n"
  "   * OUTPUT\n"
  "   */\n"
  "  gl_FragColor = bloom * gradient * noise;\n"
  "}\n";
#endif

