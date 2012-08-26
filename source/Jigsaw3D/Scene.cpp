#include "Scene.h"

#include "Renderer.h"
#include "SceneItem.h"

Scene::Scene() :
	m_depthProgram("GPUPrograms/depth.vs", "GPUPrograms/depth.fs"),
	m_depthTexture(Renderer::getInstance()->getScreenWidth(), 
				   Renderer::getInstance()->getScreenHeight(), 
				   Texture::InternalFormat::RGBA8)
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
	Renderer* renderer = Renderer::getInstance();
	renderer->setBlendMode(Renderer::BlendMode::NoBlend);
	renderer->setTextureRenderTarget(&m_depthTexture, true);
	m_depthProgram.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->draw();
	}

	renderer->setTextureRenderTarget(NULL, true);
	m_depthTexture.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->getGPUProgram()->select();
		(*it)->draw();
	}
}
