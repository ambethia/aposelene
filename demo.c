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

#define ZERO_VELOCITY 40

int fauxWidth = 100;
int fauxHeight = 75;

int realWidth = 400;
int realHeight = 300;

ASTexture *texture;
ASFont *font;
ASSprite *coin;

static void init()
{
  texture = asTextureCreate(coin_texture);
  ASVector2Df position = ASVector2DfMake(fauxWidth / 2.0f, fauxHeight / 2.0f);
  coin = asSpriteCreate(texture, coin_spin_animation, position);
  coin->velocity = ASVector2DfMake(0.0f, -15.0f);

//  font = asFontCreate(NULL);
//  ASText *text = asTextCreate("Hello, World!", ASVector2DfMake(1.0f, 1.0f), font);
}

static void update(double deltaTime)
{
  if(coin->position.x > fauxWidth)
    coin->position.x = -16.0f;

  if(coin->position.y < -16.0f)
    coin->position.y = fauxHeight;
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

  asRendererCallback(update);

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
