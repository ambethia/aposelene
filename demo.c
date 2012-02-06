#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glfw.h>

int width = 320;
int height = 240;

#define DRAW_RATE 0.001f

GLuint texture;

struct {
  GLfloat x;
  GLfloat y;
} position;

struct {
  GLuint x;
  GLuint y;
} size;

typedef struct {
  GLfloat x;
  GLfloat y;
} Vector2D;

typedef struct {
  Vector2D v0;
  Vector2D v1;
  Vector2D v2;
  Vector2D v3;
} Quad;

GLfloat frameIndex;

// u,v,width,height
const unsigned int zero_texcoords[] = {
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
};

#define ZERO_BITMAP_WIDTH 128
#define ZERO_BITMAP_HEIGHT 256
#define ZERO_NUM_IMAGES 10
#define ZERO_VELOCITY 60
#define ZERO_FRAME_STEP 10

#define INVERT_TEXTURE_COORDS 1

void init()
{
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

  // Initialize Camera
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glOrtho(0, width, 0, height, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  if(glfwLoadTexture2D("zero.tga", 0)) { 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  } else exit(EXIT_FAILURE);

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  size.x = 52;
  size.y = 46;

  // Setup sprite
  position.x = 0.0f; //width / 2 - size.x / 2;
  position.y = 0.0f; //height / 2 - size.y / 2;

  frameIndex = 0.0f;

}

void draw(double deltaTime)
{
  glClear(GL_COLOR_BUFFER_BIT);

  position.x += ZERO_VELOCITY * deltaTime;
  if(position.x > width)
  {
    position.x = 1.0f - size.x;
  }
  // position.y += ZERO_VELOCITY * deltaTime;

  if(frameIndex >= ZERO_NUM_IMAGES - 1)
  {
    frameIndex = 0.0f;
  }

  Quad vertices;
  Quad coordinates;

  GLfloat xRatio = 1.0f / ZERO_BITMAP_WIDTH;
  GLfloat yRatio = 1.0f / ZERO_BITMAP_HEIGHT;

  int sIndex = round(frameIndex) * 4;
  int tIndex = round(frameIndex) * 4 + 1;

  int xOffset = zero_texcoords[sIndex];
  int yOffset = zero_texcoords[tIndex];

  GLfloat s0 = xRatio * xOffset;
  GLfloat t0 = yRatio * yOffset;
  GLfloat s1 = xRatio * size.x + s0;
  GLfloat t1 = yRatio * size.y + t0;

  vertices.v0.x = position.x;
  vertices.v0.y = position.y;
  vertices.v1.x = position.x + size.x;
  vertices.v1.y = position.y;
  vertices.v2.x = position.x;
  vertices.v2.y = position.y + size.y;
  vertices.v3.x = position.x + size.x;
  vertices.v3.y = position.y + size.y;

  if (INVERT_TEXTURE_COORDS)
  {
    coordinates.v0.x = s0;
    coordinates.v0.y = 1.0f - t1;
    coordinates.v1.x = s1;
    coordinates.v1.y = 1.0f - t1;
    coordinates.v2.x = s0;
    coordinates.v2.y = 1.0f - t0;
    coordinates.v3.x = s1;
    coordinates.v3.y = 1.0f - t0;
  }
  else
  {
    coordinates.v0.x = s0;
    coordinates.v0.y = 1.0f - t0;
    coordinates.v1.x = s1;
    coordinates.v1.y = 1.0f - t0;
    coordinates.v2.x = s0;
    coordinates.v2.y = 1.0f - t1;
    coordinates.v3.x = s1;
    coordinates.v3.y = 1.0f - t1;
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(2, GL_FLOAT, 0, &vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, &coordinates);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  frameIndex += 1.0f * (deltaTime * ZERO_FRAME_STEP);
}

int main (int argc, char const *argv[])
{
  int running = GL_TRUE;

  if (!glfwInit())
    exit(EXIT_FAILURE);

  // Open an OpenGL window
  if (!glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowTitle("");

  init();

  double lastTime = glfwGetTime();

  while (running) {
    double thisTime = glfwGetTime();
    double deltaTime = thisTime - lastTime;

    if(deltaTime >= DRAW_RATE)
    {
      lastTime = thisTime;
      draw(deltaTime);
      glfwSwapBuffers();
    }

    running = !glfwGetKey(GLFW_KEY_ESC) &&
      glfwGetWindowParam(GLFW_OPENED);
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
