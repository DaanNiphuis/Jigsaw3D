#ifndef SCENEITEM_H
#define SCENEITEM_H

#include "Color.h"
#include "Vector3.h"

#include <vector>

class GPUProgram;

class SceneItem
{
public:
	SceneItem();
	virtual ~SceneItem();

	virtual void update(float p_timePassed) = 0;

	inline const GPUProgram* getGPUProgram() const {return m_GPUProgram;}

	void createGPUProgram();
	void destroyGPUProgram();
	void updateGPUProgram();

	inline void setPosition(const Vector3& p_position) {m_position = p_position;}
	inline void setRotation(const Vector3& p_rotation) {m_rotation = p_rotation;}
	inline void setScale(const Vector3& p_scale) {m_scale = p_scale;}

protected:
	virtual void createGPUProgramImpl() = 0;
	virtual void destroyGPUProgramImpl() = 0;
	virtual void updateGPUProgramImpl() = 0;

	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;

	std::vector<Vector3> m_positions;
	std::vector<Vector3> m_normals;
	std::vector<Color> m_colors;
	std::vector<unsigned int> m_indices;

	GPUProgram* m_GPUProgram;

	friend class Renderer;
};

#endif
