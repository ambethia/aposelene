//
//  renderer.h
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#ifndef aposelene_renderer_h
#define aposelene_renderer_h

void asInit(int width, int height);

void asRender(double deltaTime);

void asRendererReshape(int width, int height);

void asRendererCallback(void (*renderCallback)(double));

void asTerminate();

#endif
