//
//  texture.h
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#ifndef aposelene_texture_h
#define aposelene_texture_h

#include "common.h"

typedef struct ASTexture {
  GLuint name;
  ASVector2D size;
} ASTexture;

typedef struct ASTextureResource {
  int width;
  int height;
  unsigned int *pixels;
} ASTextureResource;


ASTexture * asTextureCreate(ASTextureResource *resource);

void asTextureDestroy(ASTexture *self);

void asTextureDrawImmediate(ASTexture *self, ASRect frame, ASVector2D position);

#endif
