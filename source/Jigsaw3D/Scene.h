#ifndef SCENE_H
#define SCENE_H

#include "Texture.h"
#include "GPUProgram.h"

#include <vector>

class SceneItem;

class Scene
{
public:
	Scene();
	~Scene();

	// Added sceneitems are owned by this scene and will be destroyed when the scene is destroyed.
	void add(SceneItem* p_sceneItem);

	void update(float p_timePassed);
	void draw() const;

private:
	typedef std::vector<SceneItem*> SceneItems;
	SceneItems sceneItems;

	GPUProgram m_depthProgram;
	Texture m_depthTexture;
};

#endif
