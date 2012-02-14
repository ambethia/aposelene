//
//  sprite.c
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include <math.h>
#include <stdlib.h>

#include "sprite.h"

static ASSprite *alphaSprite;
static ASSprite *omegaSprite;

enum ASAtlasOffset {
  AS_ATLAS_X,
  AS_ATLAS_Y,
  AS_ATLAS_WIDTH,
  AS_ATLAS_HEIGHT,
  AS_ATLAS_CLIP_X,
  AS_ATLAS_CLIP_Y,
  AS_ATLAS_CLIP_WIDTH,
  AS_ATLAS_CLIP_HEIGHT,
  AS_ATLAS_OFFSET
};

ASSprite * asSpriteCreate(ASTexture *texture, ASSpriteAnimation *animation, ASVector2Df position)
{
  ASSprite *sprite; 
  sprite = (ASSprite *)malloc(sizeof(ASSprite));

  sprite->texture = texture;
  sprite->animation = animation;
  sprite->position = position;
  sprite->velocity = ASVector2DfMake(0.0f, 0.0f);
  sprite->frame = animation->frameCount; // ensures we start at 0 in the update
  sprite->link = NULL;

  // First sprite?
  if (!alphaSprite)
    alphaSprite = sprite;

  // Add to the end.
  if (!omegaSprite) {
    omegaSprite = sprite;
  } else {
    omegaSprite->link = sprite;
  }

  return sprite;
}

void asSpriteDestroy(ASSprite *self)
{
  if(self)
    free(self);
}

void asSpriteUpdate(double deltaTime)
{
  ASSprite *sprite = alphaSprite;
  while (sprite) {
    // Update position
    sprite->position.x += sprite->velocity.x * deltaTime;
    sprite->position.y += sprite->velocity.y * deltaTime;

    // Update animation
    if(sprite->frame >= sprite->animation->frameCount - 1)
      sprite->frame = 0.0f;
    sprite->frame += (deltaTime * sprite->animation->speed);

    sprite = sprite->link;
  }
}

void asSpriteDraw(void)
{
  ASSprite *sprite = alphaSprite;
  while (sprite) {
    int frame = sprite->animation->frameIndex[(int)floor(sprite->frame)];
    int *atlas = &sprite->animation->atlas[frame * AS_ATLAS_OFFSET];
    int s = *(atlas + AS_ATLAS_X);
    int t = *(atlas + AS_ATLAS_Y);
    int w = *(atlas + AS_ATLAS_CLIP_WIDTH);
    int h = *(atlas + AS_ATLAS_CLIP_HEIGHT);
    int x = *(atlas + AS_ATLAS_CLIP_X) + round(sprite->position.x);
    int y = *(atlas + AS_ATLAS_CLIP_Y) + round(sprite->position.y);

    ASRect rect = ASRectMake(s, t, w, h);
    ASVector2D position = ASVector2DMake(x, y);
    asTextureDrawImmediate(sprite->texture, rect, position);
    sprite = sprite->link;
  }
}