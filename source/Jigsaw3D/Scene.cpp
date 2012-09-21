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
				   Texture::InternalFormat::RGBA8),
	m_showAmbienOcclusion(true)
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
	m_ssaaProgram.setUniformVariable("colorTexture", TextureSlot::Texture0);
	m_ssaaProgram.setUniformVariable("depthNormalTexture", TextureSlot::Texture1);
	m_ssaaProgram.setUniformVariable("backDepthTexture", TextureSlot::Texture2);
	ssaaDepthTexLoc = m_ssaaProgram.getUniformLocation("depthTexture");
	ssaaBackDepthTexLoc = m_ssaaProgram.getUniformLocation("backDepthTexture");
	ssaaNormalTexLoc = m_ssaaProgram.getUniformLocation("normalTexture");

	// set up full screen quad
	float m_fsqPositions[4 * 2];
	m_fsqPositions[0] = -1;
	m_fsqPositions[1] = 1;
	m_fsqPositions[2] = -1;
	m_fsqPositions[3] = -1;
	m_fsqPositions[4] = 1;
	m_fsqPositions[5] = 1;
	m_fsqPositions[6] = 1;
	m_fsqPositions[7] = -1;

	float m_fsqTexCoords[4 * 2];
	m_fsqTexCoords[0] = 0;
	m_fsqTexCoords[1] = 1;
	m_fsqTexCoords[2] = 0;
	m_fsqTexCoords[3] = 0;
	m_fsqTexCoords[4] = 1;
	m_fsqTexCoords[5] = 1;
	m_fsqTexCoords[6] = 1;
	m_fsqTexCoords[7] = 0;

	m_fsqBuffer.select();
	m_fsqBuffer.setVertexCount(4);
	m_fsqBuffer.setRenderMode(RenderMode::TriangleStrip);
	m_fsqBuffer.set3D(false);
	m_fsqBuffer.setVertices(reinterpret_cast<const float*>(m_fsqPositions),
							reinterpret_cast<const float*>(m_fsqTexCoords),
							NULL,
							NULL);
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

void Scene::feedKey(unsigned char p_key)
{
	if (p_key == 'a')
	{
		m_showAmbienOcclusion = !m_showAmbienOcclusion;
	}
}

void Scene::render()
{
	// Sync matrices
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->syncTransform();
	}

	Renderer* renderer = Renderer::getInstance();

	renderer->getWorldCamera()->select();

	renderer->setBlendMode(BlendMode::NoBlend);

	// Create depth/normal texture.
	renderer->setTextureRenderTarget(&m_depthNormalTexture, true);
	renderer->clearColor();
	renderer->clearDepth();
	m_depthNormalProgram.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->getVertexIndexBuffer().select();
		renderer->setWorldMatrix((*it)->getTransform());
		renderer->render();
	}

	// Create backside depth texture
	renderer->setTextureRenderTarget(&m_backDepthTexture, true);
	renderer->clearColor();
	renderer->clearDepth();
	m_backDepthProgram.select();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->getVertexIndexBuffer().select();
		renderer->setWorldMatrix((*it)->getTransform());
		renderer->render();
	}

	// ***** Render scene ******
	renderer->setBlendMode(BlendMode::AlphaBlend);
	if (m_showAmbienOcclusion)
	{
		renderer->setTextureRenderTarget(&m_accumTexture, true);
	}
	else
	{
		renderer->setTextureRenderTarget(NULL, true);
	}
	renderer->clearColor();
	renderer->clearDepth();
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->getVertexIndexBuffer().select();
		(*it)->getGPUProgram()->select();
		(*it)->updateGPUProgram();
		renderer->setWorldMatrix((*it)->getTransform());
		renderer->render();
	}

	// **** Screen space rendering *****

	// SSAA
	if (m_showAmbienOcclusion)
	{
		renderer->setWorldMatrix(Matrix44::IDENTITY);
		renderer->setBlendMode(BlendMode::NoBlend);
		renderer->useFaceCulling(false);
		renderer->setTextureRenderTarget(NULL, true);
		renderer->clearDepth();
		renderer->setWorldMatrix(Matrix44::IDENTITY);
		m_ssaaProgram.select();
		m_accumTexture.select(TextureSlot::Texture0);
		m_depthNormalTexture.select(TextureSlot::Texture1);
		m_backDepthTexture.select(TextureSlot::Texture2);
		m_fsqBuffer.select();
		renderer->render();
		renderer->useFaceCulling(true);
	}
}
