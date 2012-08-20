#ifndef TEXTURE_H
#define TEXTURE_H

#include "OpenGL.h"
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

	Texture(float p_width, float p_height);
	Texture(float p_width, float p_height, unsigned char p_byte);
	Texture(const char* p_filename);
	~Texture();

	void select() const;
	static void deselect();
	void setFilterMode(FilterMode::Enum p_filterMode);
	void setWrapMode(WrapMode::Enum p_wrapMode);

	inline unsigned int getTextureId() const {return m_texture;}
	inline float getWidth()		const {return m_width;}
	inline float getHeight()	const {return m_height;}

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

	static GLuint ms_currentTexture;

	GLuint m_texture;

	float m_width;
	float m_height;

#if !defined(_RELEASE)
	std::string m_filename;
#endif
};

#endif
