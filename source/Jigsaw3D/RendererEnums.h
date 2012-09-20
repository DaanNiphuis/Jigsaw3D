#ifndef RENDERERENUMS_H
#define RENDERERENUMS_H

#include "OpenGL.h"

struct RenderMode
{
	enum Enum
	{
		Positions = 1,
		Colors = 1 << 1,
		Textures = 1 << 2,
		Normals = 1 << 3
	};
};

struct BlendMode
{
	enum Enum
	{
		AlphaBlend,
		Additive,
		Multiply,
		NoBlend
	};
};

struct Comparison
{
	enum Enum
	{
		Never = GL_NEVER,
		Less = GL_LESS,
		LessOrEqual = GL_LEQUAL,
		Greater = GL_GREATER,
		GreaterOrEqual = GL_GEQUAL,
		Equal = GL_EQUAL,
		NotEqual = GL_NOTEQUAL,
		Allways = GL_ALWAYS
	};
};

struct StencilOperation
{
	enum Enum
	{
		Keep = GL_KEEP,
		Zero = GL_ZERO,
		Replace = GL_REPLACE,
		Increase = GL_INCR,
		Decrease = GL_DECR,
		Invert = GL_INVERT
	};
};

struct CullingFace
{
	enum Enum
	{
		Front = GL_FRONT,
		Back = GL_BACK,
		FrontAndBack = GL_FRONT_AND_BACK
	};
};

struct CullingFrontFace
{
	enum Enum
	{
		CCW = GL_CCW,
		CW = GL_CW
	};
};

struct TextureSlot
{
	enum Enum
	{
		Texture0,
		Texture1,
		Texture2,
		Texture3,
		Count
	};
};

#endif