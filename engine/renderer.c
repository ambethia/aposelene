//
//  renderer.c
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include <stdio.h>

#include "renderer.h"
#include "common.h"
#include "sprite.h"
#include "utility.h"

#include "post_vertex_shader.h"
#include "post_fragment_shader.h"

static struct {
  ASVector2D size;
  ASVector2D clip;
  ASVector2D position;
  GLuint fbo;
  GLuint fboTexture;
  GLuint fboVertices;
  GLuint depthBuffer;
  GLuint postShaderProgram;
  GLuint postVertexAttribute;
  GLuint fboTextureUniform;
  GLuint screenSizeUniform;
  void (*renderCallback)(double);
} renderer;

void asRendererCallback(void (*renderCallback)(double))
{
  renderer.renderCallback = renderCallback;
}

void asRendererReshape(int width, int height) {
  // Adjust scaleFactor to fit inside the screen
  int scaleFactor = 1;
  while(renderer.size.x * (scaleFactor + 1) <= width && renderer.size.y * (scaleFactor + 1) <= height)
    ++scaleFactor;
  
  renderer.clip.x = renderer.size.x * scaleFactor;
  renderer.clip.y = renderer.size.y * scaleFactor;
  
  renderer.position.x = (width - renderer.clip.x) / 2;
  renderer.position.y = (height - renderer.clip.y) / 2;
  
  glScissor(renderer.position.x, renderer.position.y, renderer.clip.x, renderer.clip.y);
}

void asRender(double deltaTime)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // Render to off screen framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
  glViewport(0, 0, renderer.size.x, renderer.size.y);
  renderer.renderCallback(deltaTime);

  // Draw all sprites
  asDrawSprites();
  
  // Render that framebuffer now with post effects
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(renderer.position.x, renderer.position.y, renderer.clip.x, renderer.clip.y);
  
  glUseProgram(renderer.postShaderProgram);
  glBindTexture(GL_TEXTURE_2D, renderer.fboTexture);
  glUniform1i(renderer.fboTextureUniform, 0);
  
  // mash them into one vector uniform;
  glUniform4f(renderer.screenSizeUniform, (float)renderer.size.x, (float)renderer.size.y, (float)renderer.clip.x, (float)renderer.clip.y);

  glEnableVertexAttribArray(renderer.postVertexAttribute); 
  glBindBuffer(GL_ARRAY_BUFFER, renderer.fboVertices);

  glVertexAttribPointer(renderer.postVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(renderer.postVertexAttribute);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
}

void asInitialize(int width, int height)
{
  renderer.size.x = width;
  renderer.size.y = height;

  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, renderer.size.x, 0, renderer.size.y, -1, 1);

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* Create back-buffer, used for post-processing */

  /* Texture */
  glGenTextures(1, &renderer.fboTexture);
  glBindTexture(GL_TEXTURE_2D, renderer.fboTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderer.size.x, renderer.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  /* Depth buffer */
  glGenRenderbuffers(1, &renderer.depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, renderer.depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, renderer.size.x, renderer.size.y);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  /* Framebuffer to link everything together */
  glGenFramebuffers(1, &renderer.fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer.fboTexture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderer.depthBuffer);
  if ((glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    fprintf(stderr, "Failed to create framebuffer.");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glGenBuffers(1, &renderer.fboVertices);
  glBindBuffer(GL_ARRAY_BUFFER, renderer.fboVertices);
  GLfloat vertices[] = {-1, -1, 1, -1, -1, 1, 1, 1};
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  /* Post-processing */
  renderer.postShaderProgram = asCreateShaderProgram(4, GL_VERTEX_SHADER, post_vertex_shader, GL_FRAGMENT_SHADER, post_fragment_shader);

  char *attribute = "position";
  renderer.postVertexAttribute = glGetAttribLocation(renderer.postShaderProgram, attribute);
  if (renderer.postVertexAttribute == -1)
    fprintf(stderr, "Could not bind attribute %s\n", attribute);

  renderer.screenSizeUniform = glGetUniformLocation(renderer.postShaderProgram, "screenSize");
  renderer.fboTextureUniform = glGetUniformLocation(renderer.postShaderProgram, "fboTexture");
}

void asTerminate(void)
{
  glDeleteRenderbuffers(1, &renderer.depthBuffer);
  glDeleteTextures(1, &renderer.fboTexture);
  glDeleteFramebuffers(1, &renderer.fbo);
  glDeleteBuffers(1, &renderer.fboVertices);
  glDeleteProgram(renderer.postShaderProgram);
}