//
//  utility.c
//  Aposelene
//
//  Copyright 2012 Jason L Perry. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>

#include "utility.h"

static GLuint asCreateShader(GLenum eShaderType, const GLchar *source)
{
  GLint length = (GLint)strlen(source);
  GLuint shader = glCreateShader(eShaderType);
  GLint status;

  if (!source)
    return 0;

  glShaderSource(shader, 1, &source, &length);
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
      case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
    }
    fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
    free(strInfoLog);
  }
  return shader;
}

// asCreateShaderProgram(2, GL_VERTEX_SHADER, "...");
// asCreateShaderProgram(4, GL_VERTEX_SHADER, "...", GL_FRAGMENT_SHADER, "...");
GLuint asCreateShaderProgram(int numArgs, ...)
{
  int numShaders = numArgs / 2;
  int i;
  GLint status;
  GLuint program = glCreateProgram();
  GLuint shaders[numShaders];
  va_list argp;

  va_start(argp, numArgs);
  for (i = 0; i < numShaders; i++) {
    GLenum type = va_arg(argp, GLenum);
    const char* shaderSource = va_arg(argp, const char*);
    shaders[i] = asCreateShader(type, shaderSource);
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

// char *match = e extractString("yoffset=-4", "yoffset=([-0-9]+)"
// match == "-4"
char *asExtractString(char *string, char *pattern)
{
  int begin, end, length, i, w=0;
  char *word = NULL;
  regex_t regex;
  regmatch_t match[2];

  regcomp(&regex, pattern, REG_EXTENDED);
  if ((regexec(&regex, string, 2, match, 0)) == 0) {
    begin = (int)match[1].rm_so;
    end = (int)match[1].rm_eo;
    length = end - begin;
    word = malloc(length+1);
    for (i = begin; i < end; i++) {
      word[w] = string[i];
      w++;
    }
    word[w] = 0;
  }
  regfree(&regex);
  return word;
}
