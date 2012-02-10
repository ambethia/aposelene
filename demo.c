//
//  demo.c
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glfw.h>

#include "aposelene.h"
#include "sprites_texture.h"

#define DRAW_RATE 0.05f

#define ZERO_NUM_FRAMES 10
#define ZERO_VELOCITY 40
#define ZERO_FRAME_STEP 10

int fauxWidth = 160;
int fauxHeight = 144;

int realWidth = 640;
int realHeight = 576;

ASTexture *texture;
ASVector2Df position;

float frameIndex;

// u,v,width,height
const unsigned int zero_texcoords[] = {
  // Run animation
  0,   0, 52, 46, // 0
 52,   0, 52, 46, // 1
  0,  46, 52, 46, // 2
 52,  46, 52, 46, // 3
  0,  92, 52, 46, // 4
 52,  92, 52, 46, // 5
  0, 138, 52, 46, // 6
  0, 184, 52, 46, // 7
 52, 138, 52, 46, // 8
 52, 184, 52, 46, // 9
 // Background
  0, 233, 160, 144
};

void init()
{
  texture = asTextureCreate(sprites_texture);
  position.x = 0.0f;
  position.y = 6.0f;
  frameIndex = 0.0f;
}

void draw(double deltaTime)
{

  position.x += ZERO_VELOCITY * deltaTime;
  if(position.x > fauxWidth)
  {
    position.x = 1.0f - 52.0f;
  }

  if(frameIndex >= ZERO_NUM_FRAMES - 1)
  {
    frameIndex = 0.0f;
  }

  int frame = round(frameIndex);
  int sIndex = zero_texcoords[frame * 4];
  int tIndex = zero_texcoords[frame * 4 + 1];
  int xOffset = zero_texcoords[frame * 4 + 2];
  int yOffset = zero_texcoords[frame * 4 + 3];
  
  ASRect background = ASRectMake(zero_texcoords[40], zero_texcoords[41], zero_texcoords[42], zero_texcoords[43]);
  ASRect zero = ASRectMake(sIndex, tIndex, xOffset, yOffset);

  asTextureDraw(texture, background, ASVector2DMake(0, 0));
  asTextureDraw(texture, zero, ASVector2DMake(round(position.x), round(position.y)));

  frameIndex += 1.0f * (deltaTime * ZERO_FRAME_STEP);
}
int main (int argc, char const *argv[])
{
  if (!glfwInit())
    exit(EXIT_FAILURE);

  // Open an OpenGL window
  if (!glfwOpenWindow(realWidth, realHeight, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowTitle("");

  asInit(fauxWidth, fauxHeight);

  glfwSetWindowSizeCallback(asRendererReshape);

  init();

  asRendererCallback(draw);

  int running = GL_TRUE;
  double lastTime = glfwGetTime();
  double elapsedTime = 0.0f;
  
  while (running) {
    double thisTime = glfwGetTime();
    double deltaTime = thisTime - lastTime;
    
    if(deltaTime >= DRAW_RATE)
    {
      lastTime = thisTime;
      asRender(deltaTime);
      glfwSwapBuffers();
    }
    elapsedTime += deltaTime;
    
    running = !glfwGetKey(GLFW_KEY_ESC) &&
    glfwGetWindowParam(GLFW_OPENED);
  }

  asTerminate();

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
