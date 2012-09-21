#ifndef PUZZLEVISUAL_H
#define PUZZLEVISUAL_H

#include "Color.h"
#include "common.h"
#include "SceneItem.h"
#include "Vector3.h"

#include <vector>

struct Placement;
class PuzzleLayout;
class PuzzlePiece;

class PuzzleVisual : public SceneItem
{
public:
	PuzzleVisual(); 
	PuzzleVisual(const PuzzleLayout& p_puzzleLayout);
	virtual ~PuzzleVisual();

	virtual void update(float p_timePassed);

	void addVertexData(); // Random cube locations.
	void addVertexData(const PuzzleLayout& p_puzzleLayout);
	void addVertexData(const PuzzleLayout& p_puzzleLayout, Location::Enum p_location);

protected:
	virtual void createGPUProgramImpl();
	virtual void destroyGPUProgramImpl();
	virtual void updateGPUProgramImpl();

private:
	PuzzleVisual& operator=(const PuzzleVisual&){return *this;}

	void clearVertexData();
	// add cube centered at x, y, z
	void addCube(const Vector3& p_position, const Color& color);

	int m_lightColLocation;
	int m_lightDirLocation;
	int m_lightAmbientLocation;
	int m_camPosLocation;
	int m_roughnessLocation;
	int m_albedoLocation;

	std::vector<Vector3> m_positions;
	std::vector<Vector3> m_normals;
	std::vector<Color> m_colors;
	std::vector<unsigned int> m_indices;
};

#endif
