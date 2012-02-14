//
//  renderer.h
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#ifndef aposelene_renderer_h
#define aposelene_renderer_h

void asInit(int width, int height, int scale, void (*initCallback)(void), void (*updateCallback)(float, float));

void asInitCallback(void (*initCallback)(void));

void asUpdateCallback(void (*updateCallback)(float, float));

void asMain(void);

void asRender(float deltaTime, float elapsedTime);

void asTerminate(void);

void asReshape(int width, int height);

#endif
