#ifndef SCENEITEM_H
#define SCENEITEM_H

class GPUProgram;

class SceneItem
{
public:
	SceneItem();
	virtual ~SceneItem();

	virtual void update(float p_timePassed) = 0;
	virtual void draw() const = 0;

	inline const GPUProgram* getGPUProgram() const {return m_GPUProgram;}

protected:
	GPUProgram* m_GPUProgram;
};

#endif
