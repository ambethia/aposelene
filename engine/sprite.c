//
//  sprite.c
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>

#include "sprite.h"

ASSprite * asSpriteCreate(ASTexture *texture, ASVector2Df position, float frame)
{
  ASSprite *sprite; 
  sprite = (ASSprite *)malloc(sizeof(ASSprite));

  sprite->texture = texture;
  sprite->position = position;
  sprite->frame = frame;

  // First sprite?
  if (ASSpriteList.alpha == NULL)
    ASSpriteList.alpha = sprite;

  // Add to the end.
  if (ASSpriteList.omega == NULL) {
    ASSpriteList.omega = sprite;
  } else {
    ASSpriteList.omega->link = sprite;
  }

  return sprite;
}

void asSpriteDestroy(ASSprite *self)
{
  if(self)
    free(self);
}

void asDrawSprites(void)
{
//  TODO: For each sprite:
  
//  int frame = round(coin->frame);
//  int sIndex = zero_texcoords[frame * 4];
//  int tIndex = zero_texcoords[frame * 4 + 1];
//  int xOffset = zero_texcoords[frame * 4 + 2];
//  int yOffset = zero_texcoords[frame * 4 + 3];
//  ASRect rect = ASRectMake(sIndex, tIndex, xOffset, yOffset);
//  asTextureDrawImmediate(texture, rect, ASVector2DMake(round(zero->position.x), round(zero->position.y)));

}