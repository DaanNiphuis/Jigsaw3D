#include "PuzzleVisual.h"

#include "GPUProgram.h"
#include "Puzzle.h"
#include "PuzzleLayout.h"
#include "PuzzlePiece.h"
#include "MathFunctions.h"
#include "Renderer.h"

PuzzleVisual::PuzzleVisual()
{
	updateVertexData();
}

PuzzleVisual::PuzzleVisual(const PuzzleLayout& p_puzzleLayout)
{
	updateVertexData(p_puzzleLayout);
}


PuzzleVisual::~PuzzleVisual()
{
	
}

void PuzzleVisual::update(float /*p_timePassed*/)
{

}

void PuzzleVisual::render() const
{
	Renderer* renderer = Renderer::getInstance();	

	if (m_indices.empty())
		return;

	renderer->render(reinterpret_cast<const float*>(&m_positions[0]), 
					 NULL, 
					 reinterpret_cast<const float*>(&m_colors[0]), 
					 reinterpret_cast<const float*>(&m_normals[0]), 
					 reinterpret_cast<const unsigned int*>(&m_indices[0]), 
					 m_indices.size(), true, false);
}

void PuzzleVisual::updateVertexData()
{
	clearVertexData();

	const unsigned int width = 5;
	const unsigned int depth = 5;
	const unsigned int height = 5;

	const float xOffset = static_cast<float>(width) * -0.5f + 0.5f;
	const float yOffset = static_cast<float>(height) * -0.5f + 0.5f;
	const float zOffset = static_cast<float>(depth) * -0.5f + 0.5f;

	std::vector<std::vector<unsigned int> > pieces;

	for (unsigned int i = 0; i < height; ++i)
	{
		pieces.push_back(std::vector<unsigned int>());
		for (unsigned int j = 0; j < depth; ++j)
		{
			for (unsigned int k = 0; k < width; ++k)
			{
				pieces[i].push_back(Math::randomBetween(0u, 1u));
			}
		}
	}

	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < depth; ++j)
		{
			for (unsigned int k = 0; k < width; ++k)
			{
				const unsigned int index = k + width * j;
				if (pieces[i][index] > 0)
				{
					addCube(float(k) + xOffset, float(i) + yOffset, float(j) + zOffset);
				}
			}
		}
	}
}

void PuzzleVisual::updateVertexData(const PuzzleLayout& /*p_puzzleLayout*/)
{
	clearVertexData();

	//const Puzzle& puzzle = p_puzzleLayout.get_puzzle();
	//const Placement* placedPieces = p_puzzleLayout.get_placed_pieces();
	//const unsigned int size = puzzle.get_gridwidth();

	//const Placement& top = placedPieces[Location::Top];
	//const PuzzlePiece* topPiece = puzzle.get_piece(top.piece_index);
}

void PuzzleVisual::createGPUProgramImpl()
{
	m_GPUProgram = new GPUProgram("GPUPrograms/orennayar.vs", "GPUPrograms/orennayar.fs");
	// GPU program locations
	m_lightColLocation = m_GPUProgram->getUniformLocation("lightCol");
	m_lightDirLocation = m_GPUProgram->getUniformLocation("lightDir");
	m_lightAmbientLocation = m_GPUProgram->getUniformLocation("lightAmbient");
	m_camPosLocation = m_GPUProgram->getUniformLocation("cameraPos");
	m_roughnessLocation = m_GPUProgram->getUniformLocation("roughness");
	m_albedoLocation = m_GPUProgram->getUniformLocation("albedo");
}

void PuzzleVisual::destroyGPUProgramImpl()
{
	delete m_GPUProgram;
}

void PuzzleVisual::updateGPUProgramImpl()
{
	// update GPU program
	m_GPUProgram->select();
	m_GPUProgram->setUniformVariable(m_lightColLocation, Vector3(0.7f, 0.7f, 0.7f));
	m_GPUProgram->setUniformVariable(m_lightDirLocation, Vector3(-1.0f, -1.2f, -0.8f));
	m_GPUProgram->setUniformVariable(m_lightAmbientLocation, Vector3(0.5f, 0.5f, 0.5f));
	m_GPUProgram->setUniformVariable(m_camPosLocation, Renderer::getInstance()->getWorldCamera()->getPosition());
	m_GPUProgram->setUniformVariable(m_roughnessLocation, Math::HALF_PI);
	m_GPUProgram->setUniformVariable(m_albedoLocation, Math::HALF_PI);
}

void PuzzleVisual::clearVertexData()
{
	m_positions.clear();
	m_colors.clear();
	m_normals.clear();
	m_indices.clear();
}

void PuzzleVisual::addCube(float x, float y, float z)
{
	const float scale = 1;
	x *= scale;
	y *= scale;
	z *= scale;
	const float halfWidth = 0.5f * scale;
	const float halfDepth = 0.5f * scale;
	const float halfHeight = 0.5f * scale;

	unsigned int indexOffset = m_positions.size();

	const Color color = Color::createFromBytes(189, 140, 255,255);

	// top
	m_positions.push_back(Vector3(x-halfWidth, y + halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y + halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y + halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y + halfHeight, z-halfDepth));
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
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
	m_positions.push_back(Vector3(x-halfWidth, y - halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y - halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y - halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y - halfHeight, z+halfDepth));
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
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
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
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
	m_positions.push_back(Vector3(x+halfWidth, y+halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y-halfHeight, z+halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y-halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y+halfHeight, z-halfDepth));
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
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
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
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
	m_positions.push_back(Vector3(x+halfWidth, y+halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x+halfWidth, y-halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y-halfHeight, z-halfDepth));
	m_positions.push_back(Vector3(x-halfWidth, y+halfHeight, z-halfDepth));
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
	m_colors.push_back(color);
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
