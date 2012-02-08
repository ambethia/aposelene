//
//  font.c
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>
#include "font.h"

void asFontCreate(ASFont *self, char *fontname)
{
  ASFont *font = self;
  font = malloc(sizeof(ASFont));
  asFontColorFilter(font, ASColorMake(1.0f, 1.0f, 1.0f, 1.0f));
}

void asFontDestroy(ASFont *self)
{
  ASFont *font = self;
  if(font) {
    // TODO: free characters, etc.
    // free(font);
  } 
}
                
void asFontColorFilter(ASFont *self, ASColor color)
{
  self->colorFilter = color;
}

void asFontDraw(ASFont *self, ASVector2D position, char *text)
{
}
