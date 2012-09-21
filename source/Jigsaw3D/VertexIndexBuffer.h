#ifndef VERTEXINDEXBUFFER_H
#define VERTEXINDEXBUFFER_H

#include "RendererEnums.h"

class VertexIndexBuffer
{
public:
	VertexIndexBuffer();
	~VertexIndexBuffer();

	void select() const;

	// ***** Set up *****
	inline unsigned int getVerticesId() const {return m_vab;}
	inline unsigned int getIndicesId() const {return m_eab;}
	inline unsigned int getVertexCount() const {return m_vertexCount;}
	inline void setVertexCount(unsigned int p_count) {m_vertexCount = p_count;}
	inline unsigned int getIndicesCount() const {return m_indicesCount;}
	inline void setIndicesCount(unsigned int p_count) {m_indicesCount = p_count;}
	inline unsigned int getPositionSize() const {return m_3D ? 3 : 2;}
	inline void set3D(bool p_3D) {m_3D = p_3D;}
	inline RenderMode::Enum getRenderMode() const {return m_renderMode;}
	inline void setRenderMode(RenderMode::Enum p_mode) {m_renderMode = p_mode;}
	void setVertices(const float* p_positions, const float* p_texCoords, 
					 const float* p_normals, const float* p_colors);
	void setIndices(const unsigned int* p_indices);

	// **** Internal data *****
	inline unsigned int getPositionsSize() const {return m_positionsSize;}
	inline unsigned int getPositionsOffset() const {return 0;}
	inline unsigned int geTexCoordsSize() const {return m_texCoordsSize;}
	inline unsigned int getTexCoordsOffset() const {return m_positionsSize;}
	inline unsigned int getNormalsSize() const {return m_normalsSize;}
	inline unsigned int getNormalsOffset() const {return m_positionsSize + m_texCoordsSize;}
	inline unsigned int getColorsSize() const {return m_colorsSize;}
	inline unsigned int getColorsOffset() const {return m_positionsSize + m_texCoordsSize + m_normalsSize;}

private:
	unsigned int m_vab;
	unsigned int m_eab;

	unsigned int m_vertexCount;
	unsigned int m_indicesCount;
	bool m_3D;
	RenderMode::Enum m_renderMode;
	unsigned int m_positionsSize;
	unsigned int m_texCoordsSize;
	unsigned int m_normalsSize;
	unsigned int m_colorsSize;
};

#endif
