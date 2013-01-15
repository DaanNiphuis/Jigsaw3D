#ifndef SCENE_H
#define SCENE_H

#include "GPUProgram.h"
#include "ObjectCamera.h"
#include "Texture.h"
#include "Uncopyable.h"
#include "VertexIndexBuffer.h"

#include <vector>

class SceneItem;

class Scene : Uncopyable
{
public:
	Scene();
	~Scene();

	// Added sceneitems are owned by this scene and will be destroyed when the scene is destroyed.
	void add(SceneItem* p_sceneItem);

	void update(float p_timePassed);
	void render();

	void select();
	static void deselect();

	inline ObjectCamera& getCamera() {return m_camera;}

private:
	typedef std::vector<SceneItem*> SceneItems;
	SceneItems sceneItems;

	GPUProgram m_depthNormalProgram;
	Texture m_depthNormalTexture;
	GPUProgram m_backDepthProgram;
	Texture m_backDepthTexture;
	GPUProgram m_ssaaProgram;
	Texture m_accumTexture;
	Texture* m_noiseTexture;

	VertexIndexBuffer m_fsqBuffer;

	bool m_showAmbienOcclusion;

	ObjectCamera m_camera;
};

#endif
