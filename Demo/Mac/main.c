//
//  main.c
//  Demo
//
//  Created by Jason Perry on 2/8/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include "aposelene.h"

// Resources
#include "coin_texture.h"
#include "system_font.h"

#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 80
#define SCALE 6

static ASTexture *texture;
static ASFont *font;
static ASSprite *coin;
static ASText *text;

static void init(void)
{
  texture = asTextureCreate(coin_texture);
  ASVector2Df position = ASVector2DfMake(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  coin = asSpriteCreate(texture, coin_spin_animation, position);
  coin->velocity = ASVector2DfMake(0.0f, -15.0f);
  font = asFontCreate(127, (ASFontGlyph *)&system_font_glyphs, system_font_texture);
  text = asTextCreate("Hello, World!", ASVector2DfMake(2.0f, 8.0f), font);
}

static void update(float deltaTime, float elapsedTime)
{
  if(coin->position.x > SCREEN_WIDTH)
    coin->position.x = -16.0f;
  if(coin->position.y < -16.0f)
    coin->position.y = SCREEN_HEIGHT;
}

int main(int argc, char *argv[])
{
  asInit(SCREEN_WIDTH, SCREEN_HEIGHT, SCALE, init, update);
  asMain();
  return 0;
}
