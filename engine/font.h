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

struct ASFontCharacter {
  int characterID;
  ASRect frame;
  ASVector2D offset;
  int advance;
};
typedef struct ASFontCharacter ASFontCharacter;

struct ASFont {
  ASTexture *texture;
  ASFontCharacter *characters;
  ASColor colorFilter;
};
typedef struct ASFont ASFont;

ASFont * asFontCreate(char *fontname);

void asFontDestroy(ASFont *self);

void asFontDraw(ASFont *self, ASVector2D position, char *text);

#endif
