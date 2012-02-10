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
  GLuint name;
  ASVector2D size;
};
typedef struct ASTexture ASTexture;

struct ASTextureResource {
  int width;
  int height;
  unsigned int *pixels;
};
typedef struct ASTextureResource ASTextureResource;


ASTexture * asTextureCreate(ASTextureResource *resource);

void asTextureDestroy(ASTexture *self);

void asTextureDraw(ASTexture *self, ASRect frame, ASVector2D position);

#endif
