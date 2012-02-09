#include <stdlib.h>
#include <GL/glfw.h>

#include "aposelene.h"

static const struct {
  int height;
  int width;
  unsigned int pixels[16];
} sprites_texture = {
  4, 4,
  {
    0xff0000ff, 0x00ff00ff, 0x00ff00ff, 0x0000ffff,
    0xff0000ff, 0xffffff00, 0xffffffff, 0x0000ffff,
    0xff0000ff, 0xffffffff, 0xffffff00, 0x0000ffff,
    0xff0000ff, 0xffffff00, 0xffffffff, 0x0000ffff,
  }
};

#define SIZE 128

GLuint texture;

void init()
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, SIZE, 0, SIZE, -1, 1);
  
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
}

void draw()
{
  glClear(GL_COLOR_BUFFER_BIT);
   
  ASQuadf vertices;
  ASQuadf coordinates;
 
  vertices.v0.x = 0;    vertices.v0.y = 0;
  vertices.v1.x = SIZE; vertices.v1.y = 0;
  vertices.v2.x = 0;    vertices.v2.y = SIZE;
  vertices.v3.x = SIZE; vertices.v3.y = SIZE;
  
  coordinates.v0.x = 0.0f; coordinates.v0.y = 0.0f;
  coordinates.v1.x = 1.0f; coordinates.v1.y = 0.0f;
  coordinates.v2.x = 0.0f; coordinates.v2.y = 1.0f;
  coordinates.v3.x = 1.0f; coordinates.v3.y = 1.0f;
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(2, GL_FLOAT, 0, &vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, &coordinates);
 
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
 
}

int main (int argc, char const *argv[])
{
  int running = GL_TRUE;

  if (!glfwInit())
    exit(EXIT_FAILURE);

  if (!glfwOpenWindow(SIZE, SIZE, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowTitle("");

  init();

  while (running) {
    draw();
    glfwSwapBuffers();

    running = !glfwGetKey(GLFW_KEY_ESC) &&
      glfwGetWindowParam(GLFW_OPENED);
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
