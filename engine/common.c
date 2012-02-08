//
//  common.c
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#include "common.h"

inline ASColor ASColorMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  ASColor color;
  color.red = red;
  color.green = green;
  color.blue = blue;
  color.alpha = alpha;
  return color;
}
