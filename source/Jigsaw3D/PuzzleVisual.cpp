#include "PuzzleVisual.h"

#include "Renderer.h"

PuzzleVisual::PuzzleVisual():
	m_gpuProgram("GPUPrograms/puzzle.vs", "GPUPrograms/puzzle.fs")
{
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

	float positions[] = {0.0f, 0.0f, 0.0f, 
						 0.0f, -10.0f, 0.0f, 
						 10.0f, -10.0f, 0.0f};
	float normals[] = {0, 0, -1,
					   0, 0, -1,
					   0, 0, -1};
	unsigned int indices[] = {0, 1, 2};
	renderer->render(positions, NULL, NULL, normals, indices, 3, true, true);
}
