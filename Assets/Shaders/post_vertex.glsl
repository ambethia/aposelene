//
//  post_vertex.glsl
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

attribute vec2 position;
varying vec2 fragmentTexCoord;
 
void main(void) {
  gl_Position = vec4(position, 0.0, 1.0);
  fragmentTexCoord = (position + 1.0) / 2.0;
}
