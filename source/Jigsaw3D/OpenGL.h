#ifndef PUZZLE_OPENGL_H
#define PUZZLE_OPENGL_H

#include "GL/Glew.h"

#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN 1
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#   include <windows.h>
#endif

#include "GL/gl3.h"

void initGLExtensions();

#endif
