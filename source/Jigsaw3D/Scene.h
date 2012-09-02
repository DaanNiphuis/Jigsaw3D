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
	void render() const;

	void select();
	static void deselect();

private:
	typedef std::vector<SceneItem*> SceneItems;
	SceneItems sceneItems;

	GPUProgram m_depthProgram;
	Texture m_depthTexture;
	GPUProgram m_backDepthProgram;
	Texture m_backDepthTexture;
	GPUProgram m_normalProgram;
	Texture m_normalTexture;
	GPUProgram m_ssaaProgram;
	int ssaaDepthTexLoc;
	int ssaaBackDepthTexLoc;
	int ssaaNormalTexLoc;
	Texture m_accumTexture;
};

#endif
