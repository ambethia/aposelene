//
//  font.c
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "font.h"

ASFont * asFontCreate(char *fontName)
{
  ASFont *font; 
  font = (ASFont *)malloc(sizeof(ASFont));

  char textureName[strlen(fontName) + 4];
  strcpy(textureName, fontName);
  strcat(textureName, ".tga");
  font->texture = asTextureCreate(textureName);
  
  font->colorFilter = ASColorMake(1.0f, 1.0f, 1.0f, 1.0f);

  return font;
}

void asFontDestroy(ASFont *self)
{
  if(self) {
    asTextureDestroy(self->texture); 
    free(self);
  } 
}
                
void asFontDraw(ASFont *self, ASVector2D position, char *text)
{
}
