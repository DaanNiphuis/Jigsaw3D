#include "SceneItem.h"

#include "Debug.h"

#include <cstddef>

SceneItem::SceneItem() :
	m_GPUProgram(NULL),
	m_position(),
	m_scale(1,1,1)
{
}

SceneItem::~SceneItem()
{
}

void SceneItem::syncTransform()
{
	m_transform.setTransform(m_position, m_rotation, m_scale);
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
