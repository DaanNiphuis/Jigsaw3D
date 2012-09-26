#ifndef TEXTURE_H
#define TEXTURE_H

#include "OpenGL.h"
#include "RendererEnums.h"
#include "Uncopyable.h"

#include <string>
#include <vector>

class Texture: public Uncopyable
{
public:
	struct FilterMode
	{
		enum Enum
		{
			Point = GL_NEAREST,
			Linear = GL_LINEAR
		};
	};

	struct WrapMode
	{
		enum Enum
		{
			ClampToEdge = GL_CLAMP_TO_EDGE,
			Repeat = GL_REPEAT
		};
	};

	struct InternalFormat
	{
		enum Enum
		{
			RGBA8 = GL_RGBA8,
			RGB8 = GL_RGB8,
			RGBA32F = GL_RGBA32F,
			R32F = GL_R32F
		};
	};

	Texture(unsigned int p_width, unsigned int p_height, InternalFormat::Enum p_format);
	Texture(unsigned int p_width, unsigned int p_height, unsigned char p_byte);
	Texture(unsigned int p_width, unsigned int p_height, InternalFormat::Enum p_format, unsigned char* p_data);
	Texture(const char* p_filename);
	~Texture();

	void select(TextureSlot::Enum p_textureSlot = TextureSlot::Texture0) const;
	static void deselect();
	void setFilterMode(FilterMode::Enum p_filterMode);
	void setWrapMode(WrapMode::Enum p_wrapMode);

	inline unsigned int getId() const {return m_texture;}
	inline int getWidth()	const {return m_width;}
	inline int getHeight()	const {return m_height;}

	// Memory leak if texture allready has something loaded.
	void load(const char* p_filename);
	void unload();

	inline std::string getFilename() const 
	{
#if !defined(_RELEASE)
		return m_filename;
#else
		return "";
#endif
	}

private:
	GLuint generateTextureId() const;
	void texImage2D(InternalFormat::Enum p_format, int p_width, int p_height, const unsigned char* p_data) const;

	GLuint m_texture;

	unsigned int m_width;
	unsigned int m_height;

#if !defined(_RELEASE)
	std::string m_filename;
#endif
};

#endif
