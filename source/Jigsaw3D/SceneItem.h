#ifndef SCENEITEM_H
#define SCENEITEM_H

class GPUProgram;

class SceneItem
{
public:
	SceneItem();
	virtual ~SceneItem();

	virtual void update(float p_timePassed) = 0;
	virtual void render() const = 0;

	inline const GPUProgram* getGPUProgram() const {return m_GPUProgram;}

	void createGPUProgram();
	void destroyGPUProgram();
	void updateGPUProgram();

protected:
	virtual void createGPUProgramImpl() = 0;
	virtual void destroyGPUProgramImpl() = 0;
	virtual void updateGPUProgramImpl() = 0;

protected:
	GPUProgram* m_GPUProgram;
};

#endif
