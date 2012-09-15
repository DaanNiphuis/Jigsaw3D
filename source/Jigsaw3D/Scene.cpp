#include "Scene.h"

#include "Renderer.h"
#include "SceneItem.h"

Scene::Scene() :
	m_depthNormalProgram("GPUPrograms/depthnormal.vert", "GPUPrograms/depthnormal.frag"),
	m_depthNormalTexture(Renderer::getInstance()->getScreenWidth(), 
						 Renderer::getInstance()->getScreenHeight(), 
						 Texture::InternalFormat::RGBA32F),
	m_backDepthProgram("GPUPrograms/backDepth.vert", "GPUPrograms/backDepth.frag"),
	m_backDepthTexture(Renderer::getInstance()->getScreenWidth(), 
					   Renderer::getInstance()->getScreenHeight(), 
					   Texture::InternalFormat::RGBA8),
	m_ssaaProgram("GPUPrograms/ssaa.vert", "GPUPrograms/ssaa.frag"),
	m_accumTexture(Renderer::getInstance()->getScreenWidth(), 
				   Renderer::getInstance()->getScreenHeight(), 
				   Texture::InternalFormat::RGBA8)
{
	const Renderer* renderer = Renderer::getInstance();

	m_depthNormalProgram.select();
	m_depthNormalProgram.setUniformVariable("nearPlane", renderer->getWorldCamera()->getNearPlane());
	m_depthNormalProgram.setUniformVariable("farPlane", renderer->getWorldCamera()->getFarPlane());

	m_ssaaProgram.select();
	m_ssaaProgram.setUniformVariable("nearPlane", renderer->getWorldCamera()->getNearPlane());
	m_ssaaProgram.setUniformVariable("farPlane", renderer->getWorldCamera()->getFarPlane());
	ssaaDepthTexLoc = m_ssaaProgram.getUniformLocation("depthTexture");
	ssaaBackDepthTexLoc = m_ssaaProgram.getUniformLocation("backDepthTexture");
	ssaaNormalTexLoc = m_ssaaProgram.getUniformLocation("normalTexture");
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

	// ***** Depth and normal textures *****

	renderer->setBlendMode(Renderer::BlendMode::NoBlend);

	// Create depth/normal texture.
	renderer->setTextureRenderTarget(&m_depthNormalTexture, true);
	renderer->clearColor();
	renderer->clearDepth();
	m_depthNormalProgram.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		renderer->render(*(*it));
	}

	// Create backside depth texture
	renderer->setTextureRenderTarget(&m_backDepthTexture, true);
	renderer->clearColor();
	renderer->clearDepth();
	m_backDepthProgram.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		renderer->render(*(*it));
	}

	// ***** Render scene ******
	/*renderer->setBlendMode(Renderer::BlendMode::AlphaBlend);
	renderer->setTextureRenderTarget(NULL, true);
	renderer->clearColor();
	renderer->clearDepth();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->getGPUProgram()->select();
		(*it)->updateGPUProgram();
		renderer->render(*(*it));
	}*/

	// **** Screen space rendering *****

	// SSAA
	renderer->setBlendMode(Renderer::BlendMode::NoBlend);
	renderer->setTextureRenderTarget(NULL, true);
	renderer->clearDepth();
	m_ssaaProgram.select();
	m_depthNormalTexture.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		renderer->render(*(*it));
	}
}
