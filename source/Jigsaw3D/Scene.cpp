#include "Scene.h"

#include "Renderer.h"
#include "SceneItem.h"

Scene::Scene() :
	m_depthProgram("GPUPrograms/depth.vs", "GPUPrograms/depth.fs"),
	m_depthTexture(Renderer::getInstance()->getScreenWidth(), 
				   Renderer::getInstance()->getScreenHeight(), 
				   Texture::InternalFormat::RGBA8),
	m_backDepthProgram("GPUPrograms/backDepth.vs", "GPUPrograms/backDepth.fs"),
	m_backDepthTexture(Renderer::getInstance()->getScreenWidth(), 
					   Renderer::getInstance()->getScreenHeight(), 
					   Texture::InternalFormat::RGBA8),
	m_normalProgram("GPUPrograms/normal.vs", "GPUPrograms/normal.fs"),
	m_normalTexture(Renderer::getInstance()->getScreenWidth(), 
				    Renderer::getInstance()->getScreenHeight(), 
				    Texture::InternalFormat::RGBA8)
{
}

Scene::~Scene()
{
	for (SceneItems::iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->destroyGPUProgram();
		delete *it;
	}
}

void Scene::add(SceneItem* sceneItem)
{
	sceneItems.push_back(sceneItem);
	sceneItem->createGPUProgram();
}

void Scene::update(float p_timePassed)
{
	for (SceneItems::iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->update(p_timePassed);
	}
}

void Scene::select()
{
	Renderer::getInstance()->setScene(this);
}

void Scene::deselect()
{
	Renderer::getInstance()->setScene(NULL);
}

void Scene::render() const
{
	Renderer* renderer = Renderer::getInstance();
	renderer->setBlendMode(Renderer::BlendMode::NoBlend);

	// Create depth texture.
	renderer->setTextureRenderTarget(&m_depthTexture, true);
	m_depthProgram.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		renderer->render(*(*it));
	}

	// Create backside depth texture
	renderer->setTextureRenderTarget(&m_backDepthTexture, true);
	m_backDepthProgram.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		renderer->render(*(*it));
	}

	// Create normal texture	
	renderer->setTextureRenderTarget(&m_normalTexture, true);
	m_normalProgram.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		renderer->render(*(*it));
	}
	
	// Render scene.
	renderer->setBlendMode(Renderer::BlendMode::AlphaBlend);
	renderer->setTextureRenderTarget(NULL, true);
	m_depthTexture.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->updateGPUProgram();
		(*it)->getGPUProgram()->select();
		renderer->render(*(*it));
	}
}
