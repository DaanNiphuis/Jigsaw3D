#include "VertexBuffer.h"

#include "OpenGL.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer():
	m_vertexCount(0),
	m_3D(true),
	m_indicesCount(0),
	m_renderMode(RenderMode::Triangles)
{
	glGenBuffers(1, &m_vab);
	glGenBuffers(1, &m_eab);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_vab);
	glDeleteBuffers(1, &m_eab);
}

void VertexBuffer::select() const
{
	Renderer::getInstance()->setVertexBuffer(this);
}

void VertexBuffer::setData(const float* p_positions, const float* p_texCoords, const float* p_normals, const float* p_colors, const unsigned int* p_indices)
{
	ASSERT(m_vertexCount != 0, "Vertex count is zero.");
	ASSERT(Renderer::getInstance()->getVertexBuffer() == this, "Vertex buffer is not selected.");

	unsigned int bufferSize = 0;
	const unsigned int factor = m_3D ? 3 : 2;
	m_positionsSize = p_positions ? m_vertexCount * factor * sizeof(float) : 0;
	m_texCoordsSize = p_texCoords ? m_vertexCount * 2 * sizeof(float) : 0;
	m_normalsSize = p_normals ? m_vertexCount * 3 * sizeof(float) : 0;
	m_colorsSize = p_colors ? m_vertexCount * 4 * sizeof(float) : 0;
	bufferSize = m_positionsSize + m_texCoordsSize + m_normalsSize + m_colorsSize;

	ASSERT(bufferSize != 0, "No vertex data provided.");
	glBufferData(GL_ARRAY_BUFFER, m_positionsSize + m_texCoordsSize + m_normalsSize + m_colorsSize, NULL, GL_STATIC_DRAW);
	if (p_positions)
		glBufferSubData(GL_ARRAY_BUFFER, getPositionsOffset(), m_positionsSize, p_positions);
	if (p_texCoords)
		glBufferSubData(GL_ARRAY_BUFFER, getTexCoordsOffset(), m_texCoordsSize, p_texCoords);
	if (p_normals)
		glBufferSubData(GL_ARRAY_BUFFER, getNormalsOffset(), m_normalsSize, p_normals);
	if (p_colors)
		glBufferSubData(GL_ARRAY_BUFFER, getColorsOffset(), m_colorsSize, p_colors);

	if (p_indices)
	{
		ASSERT(m_indicesCount != 0, "Indeces count is zero.");

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesCount * sizeof(unsigned int), (const GLvoid*)p_indices, GL_STATIC_DRAW);
	}
}

