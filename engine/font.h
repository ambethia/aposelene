//
//  font.h
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#ifndef _aposelene_font_h
#define _aposelene_font_h

#include "common.h"
#include "texture.h"

typedef struct ASFontGlyph {
  int ID;
  ASRect frame;
  ASVector2D offset;
  int advance;
} ASFontGlyph;

typedef struct ASFont {
  int glyphCount;
  ASFontGlyph *glyphs;
  ASTexture *texture;
} ASFont;

ASFont * asFontCreate(int glyphCount, ASFontGlyph *glyphs, ASTextureResource *texture);

void asFontDestroy(ASFont *self);

#endif
