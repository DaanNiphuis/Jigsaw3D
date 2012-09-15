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
					   Texture::InternalFormat::R32F),
	m_ssaaProgram("GPUPrograms/ssaa.vert", "GPUPrograms/ssaa.frag"),
	m_accumTexture(Renderer::getInstance()->getScreenWidth(), 
				   Renderer::getInstance()->getScreenHeight(), 
				   Texture::InternalFormat::RGBA8)
{
	const Renderer* renderer = Renderer::getInstance();

	m_depthNormalProgram.select();
	m_depthNormalProgram.setUniformVariable("nearPlane", renderer->getWorldCamera()->getNearPlane());
	m_depthNormalProgram.setUniformVariable("farPlane", renderer->getWorldCamera()->getFarPlane());

	m_backDepthProgram.select();
	m_backDepthProgram.setUniformVariable("nearPlane", renderer->getWorldCamera()->getNearPlane());
	m_backDepthProgram.setUniformVariable("farPlane", renderer->getWorldCamera()->getFarPlane());

	m_ssaaProgram.select();
	m_ssaaProgram.setUniformVariable("nearPlane", renderer->getWorldCamera()->getNearPlane());
	m_ssaaProgram.setUniformVariable("farPlane", renderer->getWorldCamera()->getFarPlane());
	ssaaDepthTexLoc = m_ssaaProgram.getUniformLocation("depthTexture");
	ssaaBackDepthTexLoc = m_ssaaProgram.getUniformLocation("backDepthTexture");
	ssaaNormalTexLoc = m_ssaaProgram.getUniformLocation("normalTexture");

	// set up full screen quad
	m_fsqPositions[0] = -1;
	m_fsqPositions[1] = 1;
	m_fsqPositions[2] = -1;
	m_fsqPositions[3] = -1;
	m_fsqPositions[4] = 1;
	m_fsqPositions[5] = 1;
	m_fsqPositions[6] = 1;
	m_fsqPositions[7] = -1;

	m_fsqTexCoords[0] = 0;
	m_fsqTexCoords[1] = 1;
	m_fsqTexCoords[2] = 0;
	m_fsqTexCoords[3] = 0;
	m_fsqTexCoords[4] = 1;
	m_fsqTexCoords[5] = 1;
	m_fsqTexCoords[6] = 1;
	m_fsqTexCoords[7] = 0;
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

void Scene::render()
{
	Renderer* renderer = Renderer::getInstance();

	renderer->getWorldCamera()->select();

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
	renderer->setBlendMode(Renderer::BlendMode::AlphaBlend);
	renderer->setTextureRenderTarget(&m_accumTexture, true);
	renderer->clearColor();
	renderer->clearDepth();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->getGPUProgram()->select();
		(*it)->updateGPUProgram();
		renderer->render(*(*it));
	}

	// **** Screen space rendering *****

	// SSAA
	renderer->setWorldMatrix(Matrix44::IDENTITY);
	renderer->setBlendMode(Renderer::BlendMode::NoBlend);
	renderer->useFaceCulling(false);
	renderer->setTextureRenderTarget(NULL, true);
	renderer->clearDepth();
	m_ssaaProgram.select();
	m_accumTexture.select();
	renderer->render(m_fsqPositions, m_fsqTexCoords, NULL, NULL, NULL, 4, false, true);
	renderer->useFaceCulling(true);
}
