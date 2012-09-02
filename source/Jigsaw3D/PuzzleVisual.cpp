#include "PuzzleVisual.h"

#include "GPUProgram.h"
#include "Puzzle.h"
#include "PuzzleLayout.h"
#include "PuzzlePiece.h"
#include "MathFunctions.h"
#include "Renderer.h"

PuzzleVisual::PuzzleVisual()
{
	
}

PuzzleVisual::PuzzleVisual(const PuzzleLayout& p_puzzleLayout)
{
	addVertexData(p_puzzleLayout);
}


PuzzleVisual::~PuzzleVisual()
{
	
}

void PuzzleVisual::update(float /*p_timePassed*/)
{

}

void PuzzleVisual::addVertexData()
{
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
					Vector3 position(float(k) + xOffset, float(i) + yOffset, float(j) + zOffset);
					addCube(position, Color::createFromBytes(189, 140, 255,255));
				}
			}
		}
	}
}

void PuzzleVisual::addVertexData(const PuzzleLayout& p_puzzleLayout)
{
	for (unsigned int i = 0; i < Location::COUNT; ++i)
	{
		addVertexData(p_puzzleLayout, static_cast<Location::Enum>(i));
	}
}

void PuzzleVisual::addVertexData(const PuzzleLayout& p_puzzleLayout, Location::Enum p_location)
{
	const unsigned int width = p_puzzleLayout.getPuzzle().get_gridwidth();
	const unsigned int height = p_puzzleLayout.getPuzzle().get_gridwidth();
	Color color(Math::randomFloat(), Math::randomFloat(), Math::randomFloat());

	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width; ++j)
		{
			if (p_puzzleLayout.hasPoint(p_location, i, j) == 1)
			{
				// Position
				Vector3 position = p_puzzleLayout.getPointPosition(p_location, i, j);

				addCube(position, color);
			}
		}
	}
}

void PuzzleVisual::createGPUProgramImpl()
{
	m_GPUProgram = new GPUProgram("GPUPrograms/orennayar.vs", "GPUPrograms/orennayar.fs");
	m_GPUProgram->select();
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

void PuzzleVisual::addCube(const Vector3& p_position, const Color& p_color)
{
	const float x = p_position.x;
	const float y = p_position.y;
	const float z = p_position.z;
	const float halfWidth = 0.5f;
	const float halfDepth = 0.5f;
	const float halfHeight = 0.5f;

	unsigned int indexOffset = m_positions.size();

	//const Color color = Color::createFromBytes(189, 140, 255,255);
	const Color color = p_color;

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
