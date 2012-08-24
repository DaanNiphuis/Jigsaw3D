#include "OpenGL.h"

#ifdef _WIN32
#define genericGetProcAddress(x) wglGetProcAddress(x)
#elif __linux__
#define genericGetProcAddress(x) glXGetProcAddress((const GLubyte*)x)
#endif

// GL Shader functions.
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

// GL Shader variable functions.
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;

// GL Program functions.
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;

// GL framebuffer functions.
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;

// GL renderbuffer functions.
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;


void initGLExtensions()
{
	glCreateShader = (PFNGLCREATESHADERPROC) genericGetProcAddress("glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC) genericGetProcAddress("glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC) genericGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC) genericGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC) genericGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) genericGetProcAddress("glGetShaderInfoLog");

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) genericGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC) genericGetProcAddress("glUniform1i");
	glUniform1f = (PFNGLUNIFORM1FPROC) genericGetProcAddress("glUniform1f");
	glUniform2f = (PFNGLUNIFORM2FPROC) genericGetProcAddress("glUniform2f");
	glUniform3f = (PFNGLUNIFORM3FPROC) genericGetProcAddress("glUniform3f");
	glUniform4f = (PFNGLUNIFORM4FPROC) genericGetProcAddress("glUniform4f");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) genericGetProcAddress("glUniformMatrix4fv");

	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) genericGetProcAddress("glGetAttribLocation");

	glCreateProgram = (PFNGLCREATEPROGRAMPROC) genericGetProcAddress("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC) genericGetProcAddress("glDeleteProgram");
	glLinkProgram = (PFNGLLINKPROGRAMPROC) genericGetProcAddress("glLinkProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC) genericGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC) genericGetProcAddress("glDetachShader");
	glUseProgram = (PFNGLUSEPROGRAMPROC) genericGetProcAddress("glUseProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC) genericGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) genericGetProcAddress("glGetProgramInfoLog");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) genericGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) genericGetProcAddress("glDisableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) genericGetProcAddress("glVertexAttribPointer");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) genericGetProcAddress("glBindAttribLocation");

	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) genericGetProcAddress("glGenFramebuffers");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) genericGetProcAddress("glBindFramebuffer");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) genericGetProcAddress("glFramebufferTexture2D");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) genericGetProcAddress("glDeleteFramebuffers");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) genericGetProcAddress("glCheckFramebufferStatus");

	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) genericGetProcAddress("glGenRenderbuffers");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) genericGetProcAddress("glBindRenderbuffer");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) genericGetProcAddress("glRenderbufferStorage");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) genericGetProcAddress("glFramebufferRenderbuffer");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) genericGetProcAddress("glDeleteRenderbuffers");

	/*
	glCreateShader = (PFNGLCREATESHADERPROC) glXGetProcAddress((const GLubyte*)"glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC) glXGetProcAddress((const GLubyte*)"glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC) glXGetProcAddress((const GLubyte*)"glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC) glXGetProcAddress((const GLubyte*)"glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC) glXGetProcAddress((const GLubyte*)"glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) glXGetProcAddress((const GLubyte*)"glGetShaderInfoLog");

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) glXGetProcAddress((const GLubyte*)"glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC) glXGetProcAddress((const GLubyte*)"glUniform1i");
	glUniform1f = (PFNGLUNIFORM1FPROC) glXGetProcAddress((const GLubyte*)"glUniform1f");
	glUniform2f = (PFNGLUNIFORM2FPROC) glXGetProcAddress((const GLubyte*)"glUniform2f");
	glUniform3f = (PFNGLUNIFORM3FPROC) glXGetProcAddress((const GLubyte*)"glUniform3f");
	glUniform4f = (PFNGLUNIFORM4FPROC) glXGetProcAddress((const GLubyte*)"glUniform4f");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) glXGetProcAddress((const GLubyte*)"glUniformMatrix4fv");

	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) glXGetProcAddress((const GLubyte*)"glGetAttribLocation");

	glCreateProgram = (PFNGLCREATEPROGRAMPROC) glXGetProcAddress((const GLubyte*)"glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC) glXGetProcAddress((const GLubyte*)"glDeleteProgram");
	glLinkProgram = (PFNGLLINKPROGRAMPROC) glXGetProcAddress((const GLubyte*)"glLinkProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC) glXGetProcAddress((const GLubyte*)"glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC) glXGetProcAddress((const GLubyte*)"glDetachShader");
	glUseProgram = (PFNGLUSEPROGRAMPROC) glXGetProcAddress((const GLubyte*)"glUseProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC) glXGetProcAddress((const GLubyte*)"glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) glXGetProcAddress((const GLubyte*)"glGetProgramInfoLog");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) glXGetProcAddress((const GLubyte*)"glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) glXGetProcAddress((const GLubyte*)"glDisableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) glXGetProcAddress((const GLubyte*)"glVertexAttribPointer");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) glXGetProcAddress((const GLubyte*)"glBindAttribLocation");

	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) glXGetProcAddress((const GLubyte*)"glGenFramebuffers");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) glXGetProcAddress((const GLubyte*)"glBindFramebuffer");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) glXGetProcAddress((const GLubyte*)"glFramebufferTexture2D");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) glXGetProcAddress((const GLubyte*)"glDeleteFramebuffers");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) glXGetProcAddress((const GLubyte*)"glCheckFramebufferStatus");

	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) glXGetProcAddress((const GLubyte*)"glGenRenderbuffers");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) glXGetProcAddress((const GLubyte*)"glBindRenderbuffer");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) glXGetProcAddress((const GLubyte*)"glRenderbufferStorage");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) glXGetProcAddress((const GLubyte*)"glFramebufferRenderbuffer");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) glXGetProcAddress((const GLubyte*)"glDeleteRenderbuffers");
	*/
}
