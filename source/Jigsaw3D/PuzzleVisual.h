#ifndef PUZZLEVISUAL_H
#define PUZZLEVISUAL_H

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

	unsigned int m_width;
	unsigned int m_depth;
	unsigned int m_height;

	std::vector<std::vector<unsigned int> > m_pieces;
	std::vector<Vector3> m_positions;
	std::vector<Vector3> m_normals;
	std::vector<unsigned int> m_indices;
};

#endif
