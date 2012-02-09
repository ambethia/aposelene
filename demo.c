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

GLuint texture;

int renderToFBO = 1;

struct {
  GLfloat x;
  GLfloat y;
} position;

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

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D,
               0, GL_RGBA,
               sprites_texture.width,
               sprites_texture.height,
               0, GL_ABGR_EXT,
               GL_UNSIGNED_BYTE,
               sprites_texture.pixels);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

  glBindTexture(GL_TEXTURE_2D, 0);
}

void drawType()
{
  ASFont *font = asFontCreate("Assets/Fonts/system");
  asFontDraw(font, ASVector2DMake(2, 2), "This is some text.");
  asFontDestroy(font);
}

// Sprite drawing stuff that will get refactored to
// sprite objects that draw themselves.
void drawBackground()
{
  GLfloat xRatio = 1.0f / ZERO_BITMAP_WIDTH;
  GLfloat yRatio = 1.0f / ZERO_BITMAP_HEIGHT;
  
  ASQuadf vertices;
  ASQuadf coordinates;

  int xOffset = zero_texcoords[40];
  int yOffset = zero_texcoords[41];

  GLfloat s0 = xRatio * xOffset;
  GLfloat t0 = yRatio * yOffset;
  GLfloat s1 = xRatio * fauxWidth + s0;
  GLfloat t1 = yRatio * fauxHeight + t0;

  // Round off so the animation aren't "subpixel" in relation to the "super pixels".
  GLint rX = 0;
  GLint rY = 0;

  vertices.v0.x = rX;             vertices.v0.y = rY;
  vertices.v1.x = rX + fauxWidth; vertices.v1.y = rY;
  vertices.v2.x = rX;             vertices.v2.y = rY + fauxHeight;
  vertices.v3.x = rX + fauxWidth; vertices.v3.y = rY + fauxHeight;

  coordinates.v0.x = s0; coordinates.v0.y = t1;
  coordinates.v1.x = s1; coordinates.v1.y = t1;
  coordinates.v2.x = s0; coordinates.v2.y = t0;
  coordinates.v3.x = s1; coordinates.v3.y = t0;

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(2, GL_FLOAT, 0, &vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, &coordinates);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

// running animation
void drawZero()
{
  GLfloat xRatio = 1.0f / ZERO_BITMAP_WIDTH;
  GLfloat yRatio = 1.0f / ZERO_BITMAP_HEIGHT;
    
  ASQuadf vertices;
  ASQuadf coordinates;

  int sIndex = round(frameIndex) * 4;
  int tIndex = round(frameIndex) * 4 + 1;

  int xOffset = zero_texcoords[sIndex];
  int yOffset = zero_texcoords[tIndex];

  GLfloat s0 = xRatio * xOffset;
  GLfloat t0 = yRatio * yOffset;
  GLfloat s1 = xRatio * size.x + s0;
  GLfloat t1 = yRatio * size.y + t0;

  // Round off so the animation aren't "subpixel" in relation to the "super pixels".
  GLint rX = round(position.x);
  GLint rY = round(position.y);

  vertices.v0.x = rX;          vertices.v0.y = rY;
  vertices.v1.x = rX + size.x; vertices.v1.y = rY;
  vertices.v2.x = rX;          vertices.v2.y = rY + size.y;
  vertices.v3.x = rX + size.x; vertices.v3.y = rY + size.y;

  coordinates.v0.x = s0; coordinates.v0.y = t1;
  coordinates.v1.x = s1; coordinates.v1.y = t1;
  coordinates.v2.x = s0; coordinates.v2.y = t0;
  coordinates.v3.x = s1; coordinates.v3.y = t0;

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(2, GL_FLOAT, 0, &vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, &coordinates);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void draw(double deltaTime)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  position.x += ZERO_VELOCITY * deltaTime;
  if(position.x > fauxWidth)
  {
    position.x = 1.0f - size.x;
    // if(renderToFBO) {
    //   renderToFBO = 0;
    // } else {
    //   renderToFBO = 1;
    // }
  }
  // position.y += ZERO_VELOCITY * deltaTime;

  if(frameIndex >= ZERO_NUM_FRAMES - 1)
  {
    frameIndex = 0.0f;
  }

  glBindTexture(GL_TEXTURE_2D, texture);

  drawBackground();
  drawZero();
  drawType();

  glBindTexture(GL_TEXTURE_2D, 0);

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
    GL_VERTEX_SHADER, "Assets/Shaders/post.v.glsl",
    GL_FRAGMENT_SHADER, "Assets/Shaders/post.f.glsl");

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
