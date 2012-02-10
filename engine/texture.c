//
//  texture.c
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>
#include <math.h>
#include "texture.h"

ASTexture * asTextureCreate(ASTextureResource *resource)
{
  ASTexture *texture; 
  texture = (ASTexture *)malloc(sizeof(ASTexture));

  texture->size = ASVector2DMake(resource->width, resource->height);
  
  glGenTextures(1, &texture->name);
  glBindTexture(GL_TEXTURE_2D, texture->name);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resource->width, resource->height,
               0, GL_ABGR_EXT, GL_UNSIGNED_BYTE, resource->pixels);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}

void asTextureDestroy(ASTexture *self)
{
  if(self) {
    glDeleteTextures(1, &self->name);
    free(self);
  } 
}

void asTextureDraw(ASTexture *self, ASRect frame, ASVector2D position)
{
  GLfloat xRatio = 1.0f / self->size.x;
  GLfloat yRatio = 1.0f / self->size.y;
  
  ASQuadf vertices;
  ASQuadf coordinates;
  
  int xOffset = frame.origin.x;
  int yOffset = frame.origin.y;
  
  GLfloat s0 = xRatio * xOffset;
  GLfloat t0 = yRatio * yOffset;
  GLfloat s1 = xRatio * frame.size.x + s0;
  GLfloat t1 = yRatio * frame.size.y + t0;
  
  // Round off so the animation aren't "subpixel" in relation to the "super pixels".
  GLint rX = round(position.x);
  GLint rY = round(position.y);
  
  vertices.v0.x = rX;                vertices.v0.y = rY;
  vertices.v1.x = rX + frame.size.x; vertices.v1.y = rY;
  vertices.v2.x = rX;                vertices.v2.y = rY + frame.size.y;
  vertices.v3.x = rX + frame.size.x; vertices.v3.y = rY + frame.size.y;
  
  coordinates.v0.x = s0; coordinates.v0.y = t1;
  coordinates.v1.x = s1; coordinates.v1.y = t1;
  coordinates.v2.x = s0; coordinates.v2.y = t0;
  coordinates.v3.x = s1; coordinates.v3.y = t0;

  glBindTexture(GL_TEXTURE_2D, self->name);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glVertexPointer(2, GL_FLOAT, 0, &vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, &coordinates);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  glBindTexture(GL_TEXTURE_2D, 0);
}