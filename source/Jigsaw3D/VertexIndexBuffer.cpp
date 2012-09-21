#include "VertexIndexBuffer.h"

#include "OpenGL.h"
#include "Renderer.h"

VertexIndexBuffer::VertexIndexBuffer():
	m_vab(0),
	m_eab(0),
	m_vertexCount(0),
	m_3D(true),
	m_indicesCount(0),
	m_renderMode(RenderMode::Triangles)
{
}

VertexIndexBuffer::~VertexIndexBuffer()
{
	if (m_vab)
		glDeleteBuffers(1, &m_vab);
	if (m_eab)
		glDeleteBuffers(1, &m_eab);
}

void VertexIndexBuffer::select() const
{
	Renderer::getInstance()->setVertexIndexBuffer(this);
}

void VertexIndexBuffer::setVertices(const float* p_positions, const float* p_texCoords, 
									const float* p_normals, const float* p_colors)
{
	ASSERT(m_vertexCount != 0, "Vertex count is zero.");
	ASSERT(Renderer::getInstance()->getVertexIndexBuffer() == this, "Vertex buffer is not selected.");

	if (!m_vab)
	{
		glGenBuffers(1, &m_vab);
		glBindBuffer(GL_ARRAY_BUFFER, m_vab);
	}

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
}

void VertexIndexBuffer::setIndices(const unsigned int* p_indices)
{
	ASSERT(m_indicesCount != 0, "Indices count is zero.");
	ASSERT(Renderer::getInstance()->getVertexIndexBuffer() == this, "Vertex buffer is not selected.");

	if (!p_indices)
		return;

	if (!m_eab)
	{
		glGenBuffers(1, &m_eab);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eab);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesCount * sizeof(unsigned int), (const GLvoid*)p_indices, GL_STATIC_DRAW);
}

