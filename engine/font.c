//
//  font.c
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>
#include "font.h"

ASFont * asFontCreate(char *fontname)
{
  ASFont *font; 
  font = (ASFont *)malloc(sizeof(ASFont));

  font->colorFilter = ASColorMake(1.0f, 1.0f, 1.0f, 1.0f);

  return font;
}

void asFontDestroy(ASFont *self)
{
  if(self) {
    // TODO: free characters, etc.
    free(self);
  } 
}
                
void asFontDraw(ASFont *self, ASVector2D position, char *text)
{
}
