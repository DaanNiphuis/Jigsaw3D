#include "Scene.h"

#include "Input.h"
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
	m_showAmbienOcclusion(true),
	m_camera(100)
{
	m_depthNormalProgram.select();
	m_depthNormalProgram.setUniformVariable("nearPlane", m_camera.getNearPlane());
	m_depthNormalProgram.setUniformVariable("farPlane", m_camera.getFarPlane());

	m_backDepthProgram.select();
	m_backDepthProgram.setUniformVariable("nearPlane", m_camera.getNearPlane());
	m_backDepthProgram.setUniformVariable("farPlane", m_camera.getFarPlane());

	m_ssaaProgram.select();
	m_ssaaProgram.setUniformVariable("nearPlane", m_camera.getNearPlane());
	m_ssaaProgram.setUniformVariable("farPlane", m_camera.getFarPlane());
	m_ssaaProgram.setUniformVariable("colorTexture", TextureSlot::Texture0);
	m_ssaaProgram.setUniformVariable("depthNormalTexture", TextureSlot::Texture1);
	m_ssaaProgram.setUniformVariable("backDepthTexture", TextureSlot::Texture2);
	m_ssaaProgram.setUniformVariable("noiseTexture", TextureSlot::Texture3);
	
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
	m_fsqTexCoords[1] = 0;
	m_fsqTexCoords[2] = 0;
	m_fsqTexCoords[3] = 1;
	m_fsqTexCoords[4] = 1;
	m_fsqTexCoords[5] = 0;
	m_fsqTexCoords[6] = 1;
	m_fsqTexCoords[7] = 1;

	m_fsqBuffer.select();
	m_fsqBuffer.setVertexCount(4);
	m_fsqBuffer.setRenderMode(RenderMode::TriangleStrip);
	m_fsqBuffer.set3D(false);
	m_fsqBuffer.setVertices(reinterpret_cast<const float*>(m_fsqPositions),
							reinterpret_cast<const float*>(m_fsqTexCoords),
							NULL,
							NULL);

	// Create noise texture
	const unsigned int texWidth = 64;
	const unsigned int texHeight = 64;
	const unsigned int size = texWidth * texHeight * 4;
	unsigned char* data = new unsigned char[size];
	for (unsigned int i = 0; i < size; ++i)
	{
		data[i] = static_cast<unsigned char>(Math::random() % 256);
	}
	m_noiseTexture = new Texture(texWidth, texHeight, Texture::InternalFormat::RGB8, data);
	m_noiseTexture->setWrapMode(Texture::WrapMode::Repeat);
	delete[] data;
}

Scene::~Scene()
{
	for (SceneItems::iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->destroyGPUProgram();
		delete *it;
	}

	delete m_noiseTexture;
}

void Scene::add(SceneItem* sceneItem)
{
	sceneItems.push_back(sceneItem);
	sceneItem->createGPUProgram();
}

void Scene::update(float p_timePassed)
{
	m_camera.update(0);

	for (SceneItems::iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->update(p_timePassed);
	}

	if (Input::getInstance()->isPressed(Key::A))
		m_showAmbienOcclusion = !m_showAmbienOcclusion;
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
	// Sync matrices
	for (SceneItems::const_iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->syncTransform();
	}

	Renderer* renderer = Renderer::getInstance();

	m_camera.select();

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
		renderer->setActiveCamera(NULL);
		m_ssaaProgram.select();
		m_accumTexture.select(TextureSlot::Texture0);
		m_depthNormalTexture.select(TextureSlot::Texture1);
		m_backDepthTexture.select(TextureSlot::Texture2);
		m_noiseTexture->select(TextureSlot::Texture3);
		m_fsqBuffer.select();
		renderer->render();
		renderer->useFaceCulling(true);
	}
}
