#include "SceneItem.h"

#include "Debug.h"

#include <cstddef>

SceneItem::SceneItem() :
	m_GPUProgram(NULL)
{
}


SceneItem::~SceneItem()
{
}

void SceneItem::createGPUProgram()
{
	ASSERT(!m_GPUProgram, "GPUProgram allready created.");
	createGPUProgramImpl();
}

void SceneItem::destroyGPUProgram()
{
	ASSERT(m_GPUProgram, "No GPUProgram created.");
	destroyGPUProgramImpl();
}

void SceneItem::updateGPUProgram()
{
	if (m_GPUProgram)
		updateGPUProgramImpl();
}
