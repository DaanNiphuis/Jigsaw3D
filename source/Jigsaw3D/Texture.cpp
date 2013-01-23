#include "Texture.h"

#include "Debug.h"
#include "Renderer.h"

#include <algorithm>

Texture::Texture(unsigned int p_width, unsigned int p_height, Texture::InternalFormat::Enum p_format):
	m_texture(generateTextureId()),
	m_width(p_width),
	m_height(p_height)
{
	setFilterMode(FilterMode::Point);
	setWrapMode(WrapMode::ClampToEdge);	
	texImage2D(p_format, p_width, p_height, NULL);	
}

Texture::Texture(unsigned int p_width, unsigned int p_height, unsigned char p_byte):
	m_texture(generateTextureId()),
	m_width(p_width),
	m_height(p_height)
{
	unsigned char* data = new unsigned char[p_width * p_height* 4];
	std::fill(data, data + p_width * p_height * 4, p_byte);

	setFilterMode(FilterMode::Point);
	setWrapMode(WrapMode::ClampToEdge);
	texImage2D(InternalFormat::RGBA8, p_width, p_height, data);
	delete[] data;
}

Texture::Texture(unsigned int p_width, unsigned int p_height, InternalFormat::Enum p_format, unsigned char* p_data):
	m_texture(generateTextureId()),
	m_width(p_width),
	m_height(p_height)
{
	setFilterMode(FilterMode::Point);
	setWrapMode(WrapMode::ClampToEdge);
	texImage2D(p_format, p_width, p_height, p_data);
}

void Texture::unload()
{
	glDeleteTextures(1, &m_texture);
}

Texture::~Texture()
{
	unload();
}

void Texture::select(TextureSlot::Enum p_textureSlot) const
{
	Renderer::getInstance()->setTexture(this, p_textureSlot);
}

void Texture::deselect()
{
	Renderer::getInstance()->setTexture(0);
}

void Texture::setFilterMode(FilterMode::Enum p_filterMode)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, p_filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, p_filterMode);
}

void Texture::setWrapMode(WrapMode::Enum p_wrapMode)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p_wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p_wrapMode);
}

GLuint Texture::generateTextureId() const
{
	GLuint texture;
	glGenTextures(1, &texture);
	return texture;
}

void Texture::texImage2D(InternalFormat::Enum p_format, int p_width, int p_height, const unsigned char* p_data) const
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, p_format, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p_data);
}
