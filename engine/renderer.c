//
//  renderer.c
//  Aposelene
//
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include <stdio.h>

#include "renderer.h"
#include "common.h"
#include "utility.h"

#include "post_vertex_shader.h"
#include "post_fragment_shader.h"

struct {
  ASVector2D size;
  ASVector2D clip;
  ASVector2D position;
  GLuint fbo;
  GLuint fbo_texture;
  GLuint rbo_depth;
  GLuint vbo_fbo_vertices;
  GLuint program_postproc;
  GLuint attribute_v_coord_postproc;
  GLuint uniform_fbo_texture;
  GLuint uniform_screen_sizes;
  void (*renderCallback)(double);
} renderer;

void asRendererCallback(void (*renderCallback)(double))
{
  renderer.renderCallback = renderCallback;
}

void asRendererReshape(int width, int height) {

  // Adjust scaleFactor to fit inside the screen
  int scaleFactor = 1;
  while(renderer.size.x * (scaleFactor + 1) <= width &&
        renderer.size.y * (scaleFactor + 1) <= height)
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

  // Render off screen framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
  glViewport(0, 0, renderer.size.x, renderer.size.y);
  renderer.renderCallback(deltaTime);
  
  // Render that framebuffer now with post effects
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(renderer.position.x, renderer.position.y, renderer.clip.x, renderer.clip.y);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  glUseProgram(renderer.program_postproc);
  glBindTexture(GL_TEXTURE_2D, renderer.fbo_texture);
  glUniform1i(renderer.uniform_fbo_texture, 0);
  
  // mash them into one vector uniform;
  glUniform4f(renderer.uniform_screen_sizes, (float)renderer.size.x, (float)renderer.size.y,
              (float)renderer.clip.x, (float)renderer.clip.y);
  
  glEnableVertexAttribArray(renderer.attribute_v_coord_postproc); 
  glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo_fbo_vertices);
  
  glVertexAttribPointer(renderer.attribute_v_coord_postproc,  // attribute
                        2,                           // number of elements per vertex, here (x,y)
                        GL_FLOAT,                    // the type of each element
                        GL_FALSE,                    // take our values as-is
                        0,                           // no extra data between each position
                        0);                          // offset of first element
  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(renderer.attribute_v_coord_postproc);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
}

void asInit(int width, int height)
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
  glGenTextures(1, &renderer.fbo_texture);
  glBindTexture(GL_TEXTURE_2D, renderer.fbo_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderer.size.x, renderer.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  /* Depth buffer */
  glGenRenderbuffers(1, &renderer.rbo_depth);
  glBindRenderbuffer(GL_RENDERBUFFER, renderer.rbo_depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, renderer.size.x, renderer.size.y);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  
  /* Framebuffer to link everything together */
  glGenFramebuffers(1, &renderer.fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, renderer.fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer.fbo_texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderer.rbo_depth);
  if ((glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "Failed to create framebuffer.");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
  GLfloat fbo_vertices[] = {
    -1, -1,
    1, -1,
    -1,  1,
    1,  1,
  };
  glGenBuffers(1, &renderer.vbo_fbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo_fbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  /* Post-processing */
  renderer.program_postproc = asCreateShaderProgram(4,
                                           GL_VERTEX_SHADER, post_vertex_shader,
                                           GL_FRAGMENT_SHADER, post_fragment_shader);
  
  char *attribute_name = "v_coord";
  renderer.attribute_v_coord_postproc = glGetAttribLocation(renderer.program_postproc, attribute_name);
  if (renderer.attribute_v_coord_postproc == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
  }
  
  renderer.uniform_screen_sizes = glGetUniformLocation(renderer.program_postproc, "screen_sizes");
  renderer.uniform_fbo_texture = glGetUniformLocation(renderer.program_postproc, "fbo_texture");
}

void asTerminate()
{
  glDeleteRenderbuffers(1, &renderer.rbo_depth);
  glDeleteTextures(1, &renderer.fbo_texture);
  glDeleteFramebuffers(1, &renderer.fbo);
  glDeleteBuffers(1, &renderer.vbo_fbo_vertices);
  glDeleteProgram(renderer.program_postproc);
}