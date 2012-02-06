#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "utilities.h"

static void *file_contents(const char *filename, GLint *length)
{
  FILE *f = fopen(filename, "r");
  void *buffer;

  if (!f) {
    fprintf(stderr, "Unable to open %s for reading\n", filename);
    return NULL;
  }

  fseek(f, 0, SEEK_END);
  *length = ftell(f);
  fseek(f, 0, SEEK_SET);

  buffer = malloc(*length+1);
  *length = fread(buffer, 1, *length, f);
  fclose(f);
  ((char*)buffer)[*length] = '\0';

  return buffer;
}

static GLuint createShader(GLenum eShaderType  , const char *filename)
{
  GLint length;
  GLuint shader;
  GLint status;
  GLchar *source = file_contents(filename, &length);
  
  if (!source)
    return 0;
  
  shader = glCreateShader(eShaderType);
  
  glShaderSource(shader, 1, (const GLchar**)&source, &length);
  glCompileShader(shader);
  
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    GLchar *strInfoLog = malloc(infoLogLength);
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
    
    const char *strShaderType = NULL;
    switch(eShaderType) {
      case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
#ifdef GL_GEOMETRY_SHADER
      case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
#endif
      case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
    }
    fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
    free(strInfoLog);
  }
  
  return shader;
}

// createShaderProgram(2, GL_VERTEX_SHADER, "vertex.glsl");
// createShaderProgram(4, GL_VERTEX_SHADER, "vertex.glsl", GL_FRAGMENT_SHADER, "fragment.glsl");
GLuint createShaderProgram(int numArgs, ...)
{
  int numShaders = numArgs / 2;
  int i;
  GLint status;
  GLuint program;
  GLuint shaders[numShaders];
  va_list argp;

  va_start(argp, numArgs);

  program = glCreateProgram();
  
  for (i = 0; i < numShaders; i++) {
    GLenum type = va_arg(argp, GLenum);
    const char* filename = va_arg(argp, const char*);
    shaders[i] = createShader(type, filename);
    glAttachShader(program, shaders[i]);
  }
  va_end(argp);
  
  glLinkProgram(program);
  
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);    
    GLchar *strInfoLog = malloc(infoLogLength);
    glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
    fprintf(stderr, "Link failure in program:\n%s\n", strInfoLog);
    free(strInfoLog);
    
    return GL_FALSE;
  } else {
    for (i = 0; i < numShaders; i++) {
      glDeleteShader((GLuint)shaders[i]);
    }
  }
  
  return program;
}