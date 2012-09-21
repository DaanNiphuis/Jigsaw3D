#ifndef RENDERER_H
#define RENDERER_H

#include "OpenGL.h"
#include "Camera.h"
#include "Matrix44.h"
#include "RendererEnums.h"

#include <vector>

class GPUProgram;
class Material;
class Rectangle;
class Scene;
class Texture;
class Vector3;
class VertexBuffer;

class Renderer
{
public:
	static void createInstance(int p_screenWidth = 320, int p_screenHeight = 480);
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
	void useAlphaTest(bool p_useAlhpaTest);

	// Pass NULL to use the default frame buffer.
	void setTextureRenderTarget(const Texture* p_texture, bool p_useDepthBuffer);

	void clearColor() const;
	void clearDepth() const;
	void clearStencil() const;

	void beginFrame();
	void endFrame();

	const GPUProgram* getGPUProgram() const;
	void setGPUProgram(const GPUProgram* p_program);
	const VertexBuffer* getVertexBuffer() const;
	void setVertexBuffer(const VertexBuffer* p_vertexBuffer);
	void setTexture(const Texture* p_texture, TextureSlot::Enum p_textureSlot = TextureSlot::Texture0);
	void renderScene();
	//void render
	void render();

	inline void setScene(Scene* p_scene) {m_scene = p_scene;}
	inline Scene* getScene() {return m_scene;}

	inline void setActiveCamera(Camera* p_camera) {m_activeCamera = p_camera; updateCameraMatrices();}
	inline Camera* getWorldCamera() {return m_worldCamera;}
	inline const Camera* getWorldCamera() const {return m_worldCamera;}
	inline void setWorldCamera(Camera* p_camera) {m_worldCamera = p_camera;}

	inline void setWorldMatrix(const Matrix44& p_worldMatrix) {m_worldMatrix = p_worldMatrix;}
	inline void setWorldTransformation(const Vector3& p_position, const Vector3& p_rotation, const Vector3& p_scale) {m_worldMatrix.setTransformation(p_position, p_rotation, p_scale);}

	inline int getScreenWidth()	const	{return m_screenWidth;}
	inline int getScreenHeight() const	{return m_screenHeight;}

	void doGraphicsErrorCheck() const;

private:
	Renderer(int p_screenWidth, int p_screenHeight);
	~Renderer();

	void updateCameraMatrices();
	void updateWorldViewProjectionMatrix() const;

	static Renderer* ms_instance;

	Scene* m_scene;

	Camera* m_activeCamera;
	Camera* m_worldCamera;
	Matrix44 m_worldMatrix;
	Matrix44 m_viewMatrix;
	Matrix44 m_projectionMatrix;

	int m_screenWidth;
	int m_screenHeight;
	unsigned int m_renderMode;
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
	const VertexBuffer* m_vertexBuffer;
	const Texture* m_textures[TextureSlot::Count];
	const Texture* m_emptyTexture;
	TextureSlot::Enum m_currentTextureSlot;
};

#endif
