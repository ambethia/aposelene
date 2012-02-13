//
//  demo.c
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#define DRAW_RATE 0.05f

#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 75

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glfw.h>

#include "aposelene.h"
#include "coin_texture.h"
#include "system_font.h"

ASTexture *texture;
ASFont *font;
ASSprite *coin;
ASText *text;

static void init()
{
  texture = asTextureCreate(coin_texture);
  ASVector2Df position = ASVector2DfMake(42.0f, 75.0f);
  coin = asSpriteCreate(texture, coin_spin_animation, position);
  coin->velocity = ASVector2DfMake(0.0f, -15.0f);

  font = asFontCreate(127, (ASFontGlyph *)&system_font_glyphs, system_font_texture);
  text = asTextCreate("Hello, World!", ASVector2DfMake(2.0f, 8.0f), font);
}

static void update(double deltaTime)
{
  if(coin->position.x > SCREEN_WIDTH)
    coin->position.x = -16.0f;

  if(coin->position.y < -16.0f)
    coin->position.y = SCREEN_HEIGHT;
}

int main (int argc, char const *argv[])
{
  if (!glfwInit())
    exit(EXIT_FAILURE);

  // Open an OpenGL window
  if (!glfwOpenWindow(SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowTitle("");

  asInitialize(SCREEN_WIDTH, SCREEN_HEIGHT);

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
