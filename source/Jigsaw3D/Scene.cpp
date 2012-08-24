#include "Scene.h"

#include "SceneItem.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	for (SceneItems::iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		delete *it;
	}
}

void Scene::add(SceneItem* sceneItem)
{
	sceneItems.push_back(sceneItem);
}

void Scene::update(float p_timePassed)
{
	for (SceneItems::iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->update(p_timePassed);
	}
}

void Scene::draw() const
{
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->draw();
	}
}
