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

// Resources
#include "sprites_texture.h"
#include "post_vertex_shader.h"
#include "post_fragment_shader.h"

#define DRAW_RATE 0.05f
#define INVERT_TEXTURE_COORDS 0
#define ZERO_BITMAP_WIDTH 256
#define ZERO_BITMAP_HEIGHT 512
#define ZERO_NUM_FRAMES 10
#define ZERO_VELOCITY 40
#define ZERO_FRAME_STEP 10

int fauxWidth = 160;
int fauxHeight = 144;

int scaleFactor = 4;

int realWidth = 640;
int realHeight = 576;

int clipX = 0;
int clipY = 0;
int clipWidth = 640;
int clipHeight = 576;

ASTexture *texture;
ASVector2Df position;

int renderToFBO = 1;

struct {
  GLuint x;
  GLuint y;
} size;

GLfloat frameIndex;

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
  0, 234, 160, 144
};

void init()
{
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

  // Initialize Camera
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, fauxWidth, 0, fauxHeight, -1, 1);

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  texture = asTextureCreate(sprites_texture);
  
  // // Enable depth test
  // glEnable(GL_DEPTH_TEST);
  // // Accept fragment if it closer to the camera than the former one
  // glDepthFunc(GL_LESS); 

  size.x = 52;
  size.y = 46;

  // Setup sprite
  position.x = 0.0f; //width / 2 - size.x / 2;
  position.y = 6.0f; //height / 2 - size.y / 2;

  frameIndex = 0.0f;

}

void draw(double deltaTime)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  position.x += ZERO_VELOCITY * deltaTime;
  if(position.x > fauxWidth)
  {
    position.x = 1.0f - size.x;
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

/* Global */
GLuint fbo, fbo_texture, rbo_depth;
GLuint vbo_fbo_vertices;
GLuint program_postproc, attribute_v_coord_postproc,
  uniform_fbo_texture, uniform_screen_sizes;

int initFrameBuffer()
{
  /* init_resources */
  /* Create back-buffer, used for post-processing */
 
  /* Texture */
  glGenTextures(1, &fbo_texture);
  glBindTexture(GL_TEXTURE_2D, fbo_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fauxWidth, fauxHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
 
  /* Depth buffer */
  glGenRenderbuffers(1, &rbo_depth);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, fauxWidth, fauxHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
 
  /* Framebuffer to link everything together */
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
  if ((glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "Failed to create framebuffer.");
    return GL_FALSE;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  GLfloat fbo_vertices[] = {
    -1, -1,
     1, -1,
    -1,  1,
     1,  1,
  };
  glGenBuffers(1, &vbo_fbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  /* Post-processing */
  program_postproc = asCreateShaderProgram(4,
    GL_VERTEX_SHADER, post_vertex_shader,
    GL_FRAGMENT_SHADER, post_fragment_shader);

  char *attribute_name = "v_coord";
  attribute_v_coord_postproc = glGetAttribLocation(program_postproc, attribute_name);
  if (attribute_v_coord_postproc == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return GL_FALSE;
  }
 
  uniform_screen_sizes = glGetUniformLocation(program_postproc, "screen_sizes");
  uniform_fbo_texture = glGetUniformLocation(program_postproc, "fbo_texture");

  return GL_TRUE;
}

void drawFramebuffer()
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
  glUseProgram(program_postproc);
  glBindTexture(GL_TEXTURE_2D, fbo_texture);
  glUniform1i(uniform_fbo_texture, 0);

  // mash them into one vector uniform;
  glUniform4f(uniform_screen_sizes, (float)fauxWidth,
  (float)fauxHeight, (float)clipWidth, (float)clipHeight);

  glEnableVertexAttribArray(attribute_v_coord_postproc); 
  glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);

  glVertexAttribPointer(
    attribute_v_coord_postproc,  // attribute
    2,                           // number of elements per vertex, here (x,y)
    GL_FLOAT,                    // the type of each element
    GL_FALSE,                    // take our values as-is
    0,                           // no extra data between each position
    0                            // offset of first element
  );

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(attribute_v_coord_postproc);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
}

void cleanupFrameBuffer()
{
  glDeleteRenderbuffers(1, &rbo_depth);
  glDeleteTextures(1, &fbo_texture);
  glDeleteFramebuffers(1, &fbo);
  glDeleteBuffers(1, &vbo_fbo_vertices);
  glDeleteProgram(program_postproc);
}

void onReshape(int width, int height) {
  // Adjust scaleFactor to fit inside the screen
  scaleFactor = 1;
  while(fauxWidth  * (scaleFactor + 1) <= width &&
        fauxHeight * (scaleFactor + 1) <= height)
    ++scaleFactor;

  clipWidth = fauxWidth * scaleFactor;
  clipHeight = fauxHeight * scaleFactor;

  clipX = (width - clipWidth) / 2;
  clipY = (height - clipHeight) / 2;

  // This call to glViewport would be redundant because it's called in every frame.
  // glViewport(clipX, clipY, clipWidth, clipHeight);
  glScissor(clipX, clipY, clipWidth, clipHeight);
}

int main (int argc, char const *argv[])
{
  int running = GL_TRUE;

  if (!glfwInit())
    exit(EXIT_FAILURE);

  // Open an OpenGL window
  if (!glfwOpenWindow(realWidth, realHeight, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowTitle("");
  glfwSetWindowSizeCallback(onReshape);

  init();
  initFrameBuffer();

  double lastTime = glfwGetTime();
  double elapsedTime = 0.0f;

  while (running) {
    double thisTime = glfwGetTime();
    double deltaTime = thisTime - lastTime;

    if(deltaTime >= DRAW_RATE)
    {
      lastTime = thisTime;
      if(!renderToFBO) {
        draw(deltaTime);
      } else {
        // Render off screen framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, fauxWidth, fauxHeight);
        draw(deltaTime);

        // Render that framebuffer now with post effects
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(clipX, clipY, clipWidth, clipHeight);
        drawFramebuffer();          
      }
      glfwSwapBuffers();
    }
    elapsedTime += deltaTime;

    running = !glfwGetKey(GLFW_KEY_ESC) &&
      glfwGetWindowParam(GLFW_OPENED);
  }

  cleanupFrameBuffer();

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
