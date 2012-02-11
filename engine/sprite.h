//
//  sprite.h
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#ifndef aposelene_sprite_h
#define aposelene_sprite_h

#include "texture.h"

typedef struct ASSprite {
  ASTexture *texture;
  ASVector2D *position;
  ASSprite *next;
} ASSprite;

#endif
