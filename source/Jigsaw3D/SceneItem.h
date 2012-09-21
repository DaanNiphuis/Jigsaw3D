#ifndef SCENEITEM_H
#define SCENEITEM_H

#include "Matrix44.h"
#include "Vector3.h"
#include "VertexBuffer.h"

class GPUProgram;

class SceneItem
{
public:
	SceneItem();
	virtual ~SceneItem();

	virtual void update(float p_timePassed) = 0;

	inline void setPosition(const Vector3& p_position) {m_position = p_position;}
	inline void setRotation(const Vector3& p_rotation) {m_rotation = p_rotation;}
	inline void setScale(const Vector3& p_scale) {m_scale = p_scale;}
	void syncTransform();
	// Not guaranteed to be updated.
	const Matrix44& getTransform() const {return m_transform;}

	inline const GPUProgram* getGPUProgram() const {return m_GPUProgram;}

	inline const VertexBuffer& getVertexBuffer() const {return m_vertexBuffer;}

	void createGPUProgram();
	void destroyGPUProgram();
	void updateGPUProgram();

protected:
	virtual void createGPUProgramImpl() = 0;
	virtual void destroyGPUProgramImpl() = 0;
	virtual void updateGPUProgramImpl() = 0;

	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;
	Matrix44 m_transform;

	VertexBuffer m_vertexBuffer;

	GPUProgram* m_GPUProgram;

	friend class Renderer;
};

#endif
