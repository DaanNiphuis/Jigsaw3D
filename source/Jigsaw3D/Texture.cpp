#include "Texture.h"

#include <cstring>
#include "Debug.h"
#include "Renderer.h"


GLuint Texture::ms_currentTexture = 0;

Texture::Texture(float p_width, float p_height):
	m_texture(generateTextureId()),
	m_width(p_width),
	m_height(p_height)
{
	setFilterMode(FilterMode::Linear);
	setWrapMode(WrapMode::ClampToEdge);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				 static_cast<int>(p_width),
				 static_cast<int>(p_height),
				 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}

Texture::Texture(float p_width, float p_height, unsigned char p_byte):
	m_texture(generateTextureId()),
	m_width(p_width),
	m_height(p_height)
{
	int width = static_cast<int>(p_width);
	int height = static_cast<int>(p_height);
	unsigned char* data = new unsigned char[width * height* 4];
	memset(data, p_byte, width * height * 4);

	setFilterMode(FilterMode::Point);
	setWrapMode(WrapMode::ClampToEdge);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				 width, height,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete[] data;
}

void Texture::unload()
{
	glDeleteTextures(1, &m_texture);
}

Texture::~Texture()
{
	unload();
}

void Texture::select() const
{
	if (m_texture != ms_currentTexture)
	{
		ms_currentTexture = m_texture;
		Renderer::getInstance()->setTexture(this);
	}
}

void Texture::deselect()
{
	ms_currentTexture = 0;
	Renderer::getInstance()->setTexture(0);
}

void Texture::setFilterMode(FilterMode::Enum p_filterMode)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, p_filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, p_filterMode);
	glBindTexture(GL_TEXTURE_2D, ms_currentTexture);
}

void Texture::setWrapMode(WrapMode::Enum p_wrapMode)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p_wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p_wrapMode);
	glBindTexture(GL_TEXTURE_2D, ms_currentTexture);
}

GLuint Texture::generateTextureId() const
{
	GLuint texture;
	glGenTextures(1, &texture);
	return texture;
}
