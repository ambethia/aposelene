//
//  texture.h
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#ifndef aposelene_texture_h
#define aposelene_texture_h

#include "common.h"

struct ASTexture {
  GLint textureID;
  ASVector2D size;
};
typedef struct ASTexture ASTexture;

ASTexture * asTextureCreate(char *textureName);

void asTextureDestroy(ASTexture *self);

void asTextureDraw(ASTexture *self, ASRect frame, ASVector2D position);

#endif
