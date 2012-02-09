//
//  texture.c
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>
#include "texture.h"

ASTexture * asTextureCreate(char *textureName)
{
  ASTexture *texture; 
  texture = (ASTexture *)malloc(sizeof(ASTexture));
  
  return texture;
}

void asTextureDestroy(ASTexture *self)
{
  if(self) {
    // TODO: glDeleteTextures(1, &self->textureID);
    free(self);
  } 
}

void asTextureDraw(ASTexture *self, ASRect frame, ASVector2D position)
{
  
}