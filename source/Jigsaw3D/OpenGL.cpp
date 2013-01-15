#include "OpenGL.h"

#include "Debug.h"

void initGLExtensions()
{
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		PANIC(glewGetErrorString(err));
	}
}
