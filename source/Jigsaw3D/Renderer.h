#ifndef RENDERER_H
#define RENDERER_H

#include "OpenGL.h"
#include "Camera.h"
#include "Matrix44.h"

#include <vector>

class GPUProgram;
class Material;
class Rectangle;
class Scene;
class SceneItem;
class Texture;

class Renderer
{
public:
	struct RenderMode
	{
		enum Enum
		{
			Positions = 1,
			Colors = 1 << 1,
			Textures = 1 << 2,
			Normals = 1 << 3
		};
	};

	struct BlendMode
	{
		enum Enum
		{
			AlphaBlend,
			Additive,
			Multiply,
			NoBlend
		};
	};

	struct Comparison
	{
		enum Enum
		{
			Never = GL_NEVER,
			Less = GL_LESS,
			LessOrEqual = GL_LEQUAL,
			Greater = GL_GREATER,
			GreaterOrEqual = GL_GEQUAL,
			Equal = GL_EQUAL,
			NotEqual = GL_NOTEQUAL,
			Allways = GL_ALWAYS
		};
	};

	struct StencilOperation
	{
		enum Enum
		{
			Keep = GL_KEEP,
			Zero = GL_ZERO,
			Replace = GL_REPLACE,
			Increase = GL_INCR,
			Decrease = GL_DECR,
			Invert = GL_INVERT
		};
	};

	struct CullingFace
	{
		enum Enum
		{
			Front = GL_FRONT,
			Back = GL_BACK,
			FrontAndBack = GL_FRONT_AND_BACK
		};
	};

	struct CullingFrontFace
	{
		enum Enum
		{
			CCW = GL_CCW,
			CW = GL_CW
		};
	};

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

	void update(float p_timePassed);

	void clearColor() const;
	void clearDepth() const;
	void clearStencil() const;

	void beginFrame();
	void endFrame();

	void setTexture(const Texture* p_texture);
	void setGPUProgram(const GPUProgram* p_program);
	const GPUProgram* getGPUProgram() const;
	void renderScene() const;
	void render(const SceneItem& p_sceneItem);
	void render(const float* p_positions,
				const float* p_textureCoordinates,
				const float* p_colors,
				const float* p_normals,
				const unsigned int* p_indices,
				unsigned int p_vertexCount,
				bool p_3DCoordinates,
				bool p_triangleStrip);

	void startHudRendering();

	inline void setScene(Scene* p_scene) {m_scene = p_scene;}
	inline Scene* getScene() {return m_scene;}

	inline Camera* getWorldCamera() {return m_worldCamera;}
	inline const Camera* getWorldCamera() const {return m_worldCamera;}
	inline void setWorldCamera(Camera* p_camera) {m_worldCamera = p_camera;}
	inline const Camera* getHudCamera() const {return &m_hudCamera;}

	inline void setWorldMatrix(const Matrix44& p_worldMatrix) {m_worldMatrix = p_worldMatrix;}

	inline int getScreenWidth()	const	{return m_screenWidth;}
	inline int getScreenHeight() const	{return m_screenHeight;}

	void doGraphicsErrorCheck() const;

private:
	Renderer(int p_screenWidth, int p_screenHeight);
	~Renderer();

	void setCameraMatrices(const Camera* p_camera);
	void updateWorldViewProjectionMatrix() const;

	static Renderer* ms_instance;

	Scene* m_scene;

	Camera* m_worldCamera;
	Camera m_hudCamera;
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
	const GPUProgram* m_currentSelectedProgram;
	GPUProgram* m_default2DProgram;
	GPUProgram* m_default3DProgram;
	Texture* m_emptyTexture;
};

#endif
