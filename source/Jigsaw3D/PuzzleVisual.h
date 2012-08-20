#ifndef PUZZLEVISUAL_H
#define PUZZLEVISUAL_H

#include "GPUProgram.h"
#include "SceneItem.h"

class PuzzleVisual : public SceneItem
{
public:
	PuzzleVisual();
	virtual ~PuzzleVisual();

	virtual void update(float p_timePassed);
	virtual void draw() const;

private:
	GPUProgram m_gpuProgram;
};

#endif
