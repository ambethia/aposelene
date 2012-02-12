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
#include "coin_texture.h"

#define DRAW_RATE 0.05f

#define ZERO_NUM_FRAMES 10
#define ZERO_VELOCITY 40
#define ZERO_FRAME_STEP 10

int fauxWidth = 160;
int fauxHeight = 144;

int realWidth = 640;
int realHeight = 576;

ASTexture *texture;
ASSprite *coin;

static void init()
{
  coin = asSpriteCreate(texture, ASVector2DfMake(0.0f, 0.0f), 0.0f);
}

static void draw(double deltaTime)
{
  coin->position.x += ZERO_VELOCITY * deltaTime;
  if(coin->position.x > fauxWidth)
  {
    coin->position.x = -52.0f;
  }

  if(coin->frame >= ZERO_NUM_FRAMES - 1)
  {
    coin->frame = 0.0f;
  }

  coin->frame += 1.0f * (deltaTime * ZERO_FRAME_STEP);
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

  asInitialize(fauxWidth, fauxHeight);

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
