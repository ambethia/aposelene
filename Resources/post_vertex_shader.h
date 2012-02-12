//
//  post_vertex_shader.h
//  Aposelene
//
//  Generated 2012-02-12 00:35:58 -0500
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#ifndef _post_vertex_shader_h
#define _post_vertex_shader_h

static const char *post_vertex_shader =
  "attribute vec2 position;\n"
  "varying vec2 fragmentTexCoord;\n"
  "void main(void) {\n"
  "  gl_Position = vec4(position, 0.0, 1.0);\n"
  "  fragmentTexCoord = (position + 1.0) / 2.0;\n"
  "}\n";
#endif

