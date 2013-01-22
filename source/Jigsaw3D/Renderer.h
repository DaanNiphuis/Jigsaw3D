#ifndef RENDERER_H
#define RENDERER_H

#include "OpenGL.h"
#include "Matrix44.h"
#include "RendererEnums.h"
#include "RenderWindow.h"

#include <vector>

class Camera;
class GPUProgram;
class Material;
class Rectangle;
class Scene;
class Texture;
class Vector3;
class VertexIndexBuffer;

class Renderer
{
public:
	static void createInstance(const char* p_name = "", int p_screenWidth = 320, int p_screenHeight = 480);
	static Renderer* getInstance();
	static void destroyInstance();

	void defaultSettings2D();
	void defaultSettings3D();

	void setBlendMode(BlendMode::Enum p_blendMode);
	void setStencilTest(Comparison::Enum p_comparison, int p_value, unsigned int p_mask = 4294967295U);
	void setStencilOperation(StencilOperation::Enum p_fail, StencilOperation::Enum p_pass);
	void setClearColor(float r, float g, float b, float a);

	void useDepthTest(bool p_useDepthBuffer);
	void useStencilTest(bool p_useStencilTest);
	void useFaceCulling(bool p_useFaceCulling);
	void setCullingFrontFace(CullingFrontFace::Enum p_cullingFrontFace);
	void setCullingFace(CullingFace::Enum p_cullingFace);

	// Pass NULL to use the default frame buffer.
	void setTextureRenderTarget(const Texture* p_texture, bool p_useDepthBuffer);

	void clearColor() const;
	void clearDepth() const;
	void clearStencil() const;

	void beginFrame();
	void endFrame();

	inline const RenderWindow& getRenderWindow() const {return m_renderWindow;}
	const GPUProgram* getGPUProgram() const;
	void setGPUProgram(const GPUProgram* p_program);
	const VertexIndexBuffer* getVertexIndexBuffer() const;
	void setVertexIndexBuffer(const VertexIndexBuffer* p_vib);
	void setTexture(const Texture* p_texture, TextureSlot::Enum p_textureSlot = TextureSlot::Texture0);
	void renderScene();
	void render();

	inline void setScene(Scene* p_scene) {m_scene = p_scene;}
	inline Scene* getScene() {return m_scene;}

	inline void setActiveCamera(Camera* p_camera) {m_activeCamera = p_camera;}

	inline void setWorldMatrix(const Matrix44& p_worldMatrix) {m_worldMatrix = p_worldMatrix;}
	inline void setWorldTransform(const Vector3& p_position, const Vector3& p_rotation, const Vector3& p_scale) {m_worldMatrix.setTransform(p_position, p_rotation, p_scale);}

	inline int getScreenWidth()	const	{return m_screenWidth;}
	inline int getScreenHeight() const	{return m_screenHeight;}

	void doGraphicsErrorCheck() const;

private:
	Renderer(const char* p_name, int p_screenWidth, int p_screenHeight);
	~Renderer();

	void updateWorldViewProjectionMatrix() const;

	static Renderer* ms_instance;

	RenderWindow m_renderWindow;

	Scene* m_scene;

	Camera* m_activeCamera;
	Matrix44 m_worldMatrix;

	int m_screenWidth;
	int m_screenHeight;
	unsigned int m_registeredAttributes;
	int m_maxAttributes;
	BlendMode::Enum m_blendMode;
	unsigned int m_clearBits;
	unsigned int m_newClearBits;
	unsigned int m_offscreenFrameBuffer;
	unsigned int m_offscreenRenderBuffer;
	const GPUProgram* m_GPUProgram;
	GPUProgram* m_default2DProgram;
	GPUProgram* m_default3DProgram;
	unsigned int m_vao;
	const VertexIndexBuffer* m_vib;
	const Texture* m_textures[TextureSlot::Count];
	const Texture* m_emptyTexture;
	TextureSlot::Enum m_currentTextureSlot;
};

#endif
