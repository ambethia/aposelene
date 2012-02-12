//
//  sprite.h
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#ifndef aposelene_sprite_h
#define aposelene_sprite_h

#include "texture.h"

typedef struct ASSpriteAnimation {
  int *atlas;
  float speed;
  int frameCount;
  int *frameIndex;
} ASSpriteAnimation;

typedef struct ASSprite {
  ASTexture *texture;
  ASSpriteAnimation *animation;
  ASVector2Df position;
  ASVector2Df velocity;
  float frame;
  struct ASSprite *link;
} ASSprite;

ASSprite * asSpriteCreate(ASTexture *texture, ASSpriteAnimation *animation, ASVector2Df position);

void asSpriteDestroy(ASSprite *self);

void asSpriteUpdate(double deltaTime);

void asSpriteDraw(void);

#endif
