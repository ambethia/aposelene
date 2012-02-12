//
//  text.c
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>

#include "text.h"
#include "font.h"

ASText * asTextCreate(char *string, ASVector2Df position, ASFont *font)
{
  ASText *text; 
  text = (ASText *)malloc(sizeof(ASText));

  text->string = string;
  text->position = position;
  text->font = font;

  return text;
}

void asTextDestroy(ASText *self)
{
  if(self) {
    free(self);
  } 
}

void asTextDraw(void)
{
}
