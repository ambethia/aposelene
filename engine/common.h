//
//  common.h
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#ifndef _aposelene_common_h
#define _aposelene_common_h

#include <OpenGL/gl.h>

struct ASVector2D {
  GLint x;
  GLint y;
};
typedef struct ASVector2D ASVector2D;

struct ASVector2Df {
  GLfloat x;
  GLfloat y;
};
typedef struct ASVector2Df ASVector2Df;

struct ASRect {
  ASVector2D origin;
  ASVector2D size;
};
typedef struct ASRect ASRect;

struct ASRectf {
  ASVector2Df origin;
  ASVector2Df size;
};
typedef struct ASRectf ASRectf;

struct ASQuad {
  ASVector2D v0;
  ASVector2D v1;
  ASVector2D v2;
  ASVector2D v3;
};
typedef struct ASQuad ASQuad;

struct ASQuadf {
  ASVector2Df v0;
  ASVector2Df v1;
  ASVector2Df v2;
  ASVector2Df v3;
};
typedef struct ASQuadf ASQuadf;

struct ASColor {
  GLfloat red;
  GLfloat green;
  GLfloat blue;
  GLfloat alpha;
};
typedef struct ASColor ASColor;

inline ASColor ASColorMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

#endif