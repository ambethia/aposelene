//
//  text.h
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#ifndef aposelene_text_h
#define aposelene_text_h

#include "common.h"
#include "font.h"

typedef struct ASText {
  char *string;
  ASVector2Df position;
  ASFont *font;
  struct ASText *link;
} ASText;

ASText * asTextCreate(char *string, ASVector2Df position, ASFont *font);

void asTextDestroy(ASText *self);

void asTextDraw(void);

#endif
