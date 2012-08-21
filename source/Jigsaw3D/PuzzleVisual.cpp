#include "PuzzleVisual.h"

#include "MathFunctions.h"
#include "Renderer.h"

PuzzleVisual::PuzzleVisual():
	m_gpuProgram("GPUPrograms/puzzle.vs", "GPUPrograms/puzzle.fs"),
	m_width(5),
	m_depth(5),
	m_height(5)
{
	for (unsigned int i = 0; i < m_height; ++i)
	{
		m_pieces.push_back(std::vector<unsigned int>());
		for (unsigned int j = 0; j < m_depth; ++j)
		{
			for (unsigned int k = 0; k < m_width; ++k)
			{
				m_pieces[i].push_back(randomBetween(0u, 1u));
			}
		}
	}

	updateVertexData();
}


PuzzleVisual::~PuzzleVisual()
{
}

void PuzzleVisual::update(float /*p_timePassed*/)
{
}

void PuzzleVisual::draw() const
{
	Renderer* renderer = Renderer::getInstance();

	m_gpuProgram.select();

	if (m_indices.empty())
		return;

	renderer->render(reinterpret_cast<const float*>(&m_positions[0]), NULL, NULL, 
					 reinterpret_cast<const float*>(&m_normals[0]), 
					 reinterpret_cast<const unsigned int*>(&m_indices[0]), 
					 m_indices.size(), true, false);
}

void PuzzleVisual::updateVertexData()
{
	m_positions.clear();
	m_normals.clear();
	m_indices.clear();

	for (unsigned int i = 0; i < m_height; ++i)
	{
		for (unsigned int j = 0; j < m_depth; ++j)
		{
			for (unsigned int k = 0; k < m_width; ++k)
			{
				const unsigned int index = k + m_width * j;
				if (m_pieces[i][index] > 0)
				{
					addCube(float(k), float(i), float(j));
				}
			}
		}
	}
}

void PuzzleVisual::addCube(float x, float y, float z)
{
	const float scale = 10;
	x -= m_width * 0.5f;
	y -= m_height * 0.5f;
	z -= m_depth * 0.5f;
	x *= scale;
	y *= scale;
	z *= scale;
	const float halfWidth = 0.5f * scale;
	const float halfDepth = 0.5f * scale;
	const float halfHeight = 0.5f * scale;

	unsigned int indexOffset = m_positions.size();

	// top
	m_positions.push_back(Vector3(x-halfWidth, y + halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y + halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y + halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y + halfHeight, z-halfDepth));
	m_normals.push_back(Vector3(0,1,0));
	m_normals.push_back(Vector3(0,1,0));
	m_normals.push_back(Vector3(0,1,0));
	m_normals.push_back(Vector3(0,1,0));
	m_indices.push_back(0 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(2 + indexOffset);
	// bottom
	indexOffset += 4;
	m_positions.push_back(Vector3(x-halfWidth, y - halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y - halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y - halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y - halfHeight, z-halfDepth));
	m_normals.push_back(Vector3(0,-1,0));
	m_normals.push_back(Vector3(0,-1,0));
	m_normals.push_back(Vector3(0,-1,0));
	m_normals.push_back(Vector3(0,-1,0));
	m_indices.push_back(0 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(2 + indexOffset);
	// left
	indexOffset += 4;
	m_positions.push_back(Vector3(x-halfWidth, y+halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y-halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y-halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y+halfHeight, z+halfDepth));
	m_normals.push_back(Vector3(-1,0,0));
	m_normals.push_back(Vector3(-1,0,0));
	m_normals.push_back(Vector3(-1,0,0));
	m_normals.push_back(Vector3(-1,0,0));
	m_indices.push_back(0 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(2 + indexOffset);
	// right
	indexOffset += 4;
	m_positions.push_back(Vector3(x+halfWidth, y+halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y-halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y-halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y+halfHeight, z+halfDepth));
	m_normals.push_back(Vector3(1,0,0));
	m_normals.push_back(Vector3(1,0,0));
	m_normals.push_back(Vector3(1,0,0));
	m_normals.push_back(Vector3(1,0,0));
	m_indices.push_back(0 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(2 + indexOffset);
	// front
	indexOffset += 4;
	m_positions.push_back(Vector3(x-halfWidth, y+halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y-halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y-halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y+halfHeight, z+halfDepth));
	m_normals.push_back(Vector3(0,0,1));
	m_normals.push_back(Vector3(0,0,1));
	m_normals.push_back(Vector3(0,0,1));
	m_normals.push_back(Vector3(0,0,1));
	m_indices.push_back(0 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(2 + indexOffset);
	// back
	indexOffset += 4;
	m_positions.push_back(Vector3(x-halfWidth, y+halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y-halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y-halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y+halfHeight, z-halfDepth));
	m_normals.push_back(Vector3(0,0,-1));
	m_normals.push_back(Vector3(0,0,-1));
	m_normals.push_back(Vector3(0,0,-1));
	m_normals.push_back(Vector3(0,0,-1));
	m_indices.push_back(0 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(3 + indexOffset);
	m_indices.push_back(1 + indexOffset);
	m_indices.push_back(2 + indexOffset);
}
