//
//  text.c
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>

#include "text.h"
#include "font.h"

static ASText *alphaText = NULL;
static ASText *omegaText = NULL;

ASText * asTextCreate(char *string, ASVector2Df position, ASFont *font)
{
  ASText *text; 
  text = (ASText *)malloc(sizeof(ASText));

  text->string = string;
  text->position = position;
  text->font = font;
  text->link = NULL;

  // First?
  if (!alphaText)
    alphaText = text;
  
  // Add to the end.
  if (!omegaText) {
    omegaText = text;
  } else {
    omegaText->link = text;
  }
  
  return text;
}

void asTextDestroy(ASText *self)
{
  if(self) {
    free(self);
  } 
}

// TODO: Draw call every character is not very efficient, but it's easy enough for now.
// We adjust the baseline (cursor.y) to accomodate for packing in the original graphic.
void asTextDraw(void)
{
  int charID;
  int i;
  ASFontGlyph *glyph;
  ASVector2D cursor;
  ASText *text = alphaText;

  while (text) {
    cursor = ASVector2DMake(text->position.x, text->position.y);
    for (i = 0; text->string[i] != 0; ++i)
    {
      charID = text->string[i];
      glyph = &text->font->glyphs[charID];
      cursor.x += glyph->offset.x;
      cursor.y  = text->position.y - (glyph->offset.y + glyph->frame.size.y);
      asTextureDrawImmediate(text->font->texture, glyph->frame, cursor);
      cursor.x += glyph->advance;
    }
    text = text->link;
  }
}
