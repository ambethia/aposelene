//
//  post_vertex_shader.h
//  Aposelene
//
//  Generated 2012-02-10 01:37:43 -0500
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#ifndef _post_vertex_shader_h
#define _post_vertex_shader_h

static const char *post_vertex_shader =
  "attribute vec2 v_coord;\n"
  "varying vec2 f_texcoord;\n"
  "void main(void) {\n"
  "  gl_Position = vec4(v_coord, 0.0, 1.0);\n"
  "  f_texcoord = (v_coord + 1.0) / 2.0;\n"
  "}\n";
#endif

