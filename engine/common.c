//
//  common.c
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#include "common.h"

inline ASVector2D ASVector2DMake(GLint x, GLint y)
{
  ASVector2D vector2D;
  vector2D.x = x;
  vector2D.y = y;
  return vector2D;
}

inline ASVector2Df ASVector2DfMake(GLfloat x, GLfloat y)
{
  ASVector2Df vector2Df;
  vector2Df.x = x;
  vector2Df.y = y;
  return vector2Df;  
}

inline ASRect ASRectMake(GLint xOrigin, GLint yOrigin, GLint xSize, GLint ySize)
{
  ASRect rect;
  rect.origin = ASVector2DMake(xOrigin, yOrigin);
  rect.size = ASVector2DMake(xSize, ySize);
  return rect;
}

inline ASRectf ASRectfMake(GLfloat xOrigin, GLfloat yOrigin, GLfloat xSize, GLfloat ySize)
{
  ASRectf rectf;
  rectf.origin = ASVector2DfMake(xOrigin, yOrigin);
  rectf.size = ASVector2DfMake(xSize, ySize);
  return rectf;  
}

inline ASColor ASColorMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  ASColor color;
  color.red = red;
  color.green = green;
  color.blue = blue;
  color.alpha = alpha;
  return color;
}
