//
//  common.h
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#ifndef _aposelene_common_h
#define _aposelene_common_h

#include <OpenGL/gl.h>

typedef struct ASVector2D {
  GLint x;
  GLint y;
} ASVector2D;

ASVector2D ASVector2DMake(GLint x, GLint y);

typedef struct ASVector2Df {
  GLfloat x;
  GLfloat y;
} ASVector2Df;

ASVector2Df ASVector2DfMake(GLfloat x, GLfloat y);

typedef struct ASRect {
  ASVector2D origin;
  ASVector2D size;
} ASRect;

ASRect ASRectMake(GLint xOrigin, GLint yOrigin, GLint xSize, GLint ySize);

typedef struct ASRectf {
  ASVector2Df origin;
  ASVector2Df size;
} ASRectf;

ASRectf ASRectfMake(GLfloat xOrigin, GLfloat yOrigin, GLfloat xSize, GLfloat ySize);

typedef struct ASQuad {
  ASVector2D v0;
  ASVector2D v1;
  ASVector2D v2;
  ASVector2D v3;
} ASQuad;

typedef struct ASQuadf {
  ASVector2Df v0;
  ASVector2Df v1;
  ASVector2Df v2;
  ASVector2Df v3;
} ASQuadf;

typedef struct ASColor {
  GLfloat red;
  GLfloat green;
  GLfloat blue;
  GLfloat alpha;
} ASColor;

ASColor ASColorMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

#endif