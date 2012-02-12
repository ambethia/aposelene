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
  ASVector2Df position;
  float frame;
  struct ASSprite *link;
} ASSprite;

struct {
  ASSprite *alpha;
  ASSprite *omega;
} ASSpriteList;

ASSprite * asSpriteCreate(ASTexture *texture, ASVector2Df position, float frame);

void asSpriteDestroy(ASSprite *self);

void asDrawSprites(void);

#endif
