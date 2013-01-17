#include "Scene.h"

#include "GlobalProperties.h"
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
	m_depthNormalProgram.setUniform("nearPlane", m_camera.getNearPlane());
	m_depthNormalProgram.setUniform("farPlane", m_camera.getFarPlane());

	m_backDepthProgram.select();
	m_backDepthProgram.setUniform("nearPlane", m_camera.getNearPlane());
	m_backDepthProgram.setUniform("farPlane", m_camera.getFarPlane());

	m_ssaaProgram.select();
	m_ssaaProgram.setUniform("nearPlane", m_camera.getNearPlane());
	m_ssaaProgram.setUniform("farPlane", m_camera.getFarPlane());
	m_ssaaProgram.setUniform("aspectRatio", static_cast<float>(gp::SCREEN_WIDTH) / static_cast<float>(gp::SCREEN_HEIGHT));
	m_ssaaProgram.setUniform("colorTexture", TextureSlot::Texture0);
	m_ssaaProgram.setUniform("depthNormalTexture", TextureSlot::Texture1);
	m_ssaaProgram.setUniform("backDepthTexture", TextureSlot::Texture2);
	m_ssaaProgram.setUniform("noiseTexture", TextureSlot::Texture3);
	
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

	// Create noise texture
	const unsigned int texWidth = 20;
	const unsigned int texHeight = 20;
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

float projToLinDepth(float z, float n, float f)
{
	return -(2 * n * f) / (z * (f - n) - f - n);
}

float linToProjDepth(float z, float n, float f)
{
	return (((-2*n*f)/z)+f+n)/(f-n);
}

void Scene::update(float p_timePassed)
{
	m_camera.update(0);

	Vector4 pw(10,20,40,1);
	Vector4 pv = m_camera.getViewMatrix()*pw;
	Vector4 ps = m_camera.getViewProjectionMatrix()*pw;
	ps.x /= ps.w;
	ps.y /= ps.w;
	ps.z /= ps.w;
	ps.w /= ps.w;

	Vector4 psTest = m_camera.getProjectionMatrix() * pv;
	psTest.x /= psTest.w;
	psTest.y /= psTest.w;
	psTest.z /= psTest.w;
	psTest.w /= psTest.w;
	
	float tanHalfFov = std::tan(m_camera.getFov()*0.5f);
	float n = m_camera.getNearPlane();
	float f = m_camera.getFarPlane();
	float z = projToLinDepth(ps.z, n, f);
	Vector4 pv1 = Vector4(ps.x*(tanHalfFov*z),ps.y*(tanHalfFov*z),-z,1);
	Vector4 pw1 = m_camera.getViewMatrix().getInverse() * pv1;

	for (SceneItems::iterator it = sceneItems.begin(); it != sceneItems.end(); ++it)
	{
		(*it)->update(p_timePassed);
	}

	if (Input::getInstance()->isPressed(Key::A))
		m_showAmbienOcclusion = !m_showAmbienOcclusion;

	// Update GPU program data.
	if (m_showAmbienOcclusion)
	{
		m_ssaaProgram.select();
		m_ssaaProgram.setUniform("viewInv", m_camera.getViewMatrix().getInverse());
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
