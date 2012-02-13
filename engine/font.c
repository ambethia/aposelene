//
//  font.c
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>

#include "font.h"

ASFont * asFontCreate(int glyphCount, ASFontGlyph *glyphs, ASTextureResource *texture)
{
  ASFont *font; 
  font = (ASFont *)malloc(sizeof(ASFont));
  font->glyphCount = glyphCount;
  font->glyphs = glyphs;
  font->texture = asTextureCreate(texture);

  return font;
}

void asFontDestroy(ASFont *self)
{
  if(self) {
    asTextureDestroy(self->texture); 
    free(self);
  } 
}
