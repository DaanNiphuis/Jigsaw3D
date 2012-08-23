#ifndef PUZZLEVISUAL_H
#define PUZZLEVISUAL_H

#include "Color.h"
#include "GPUProgram.h"
#include "SceneItem.h"

#include <vector>

class PuzzleVisual : public SceneItem
{
public:
	PuzzleVisual();
	virtual ~PuzzleVisual();

	virtual void update(float p_timePassed);
	virtual void draw() const;

private:
	void updateVertexData();
	// add cube centered at x, y, z
	void addCube(float x, float y, float z);

	GPUProgram m_gpuProgram;
	int m_lightColLocation;
	int m_lightDirLocation;
	int m_lightAmbientLocation;
	int m_camPosLocation;
	int m_roughnessLocation;
	int m_albedoLocation;

	unsigned int m_width;
	unsigned int m_depth;
	unsigned int m_height;

	std::vector<std::vector<unsigned int> > m_pieces;
	std::vector<Vector3> m_positions;
	std::vector<Vector3> m_normals;
	std::vector<Color> m_colors;
	std::vector<unsigned int> m_indices;
};

#endif
