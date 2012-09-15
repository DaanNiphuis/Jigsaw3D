#include "Renderer.h"

#include "Debug.h"
#include "GPUProgram.h"
#include "IOFunctions.h"
#include "MathConstants.h"
#include "Scene.h"
#include "SceneItem.h"
#include "Texture.h"

#include <string>

Renderer* Renderer::ms_instance = NULL;

void Renderer::createInstance(int p_screenWidth, int p_screenHeight)
{
	if (ms_instance == NULL)
	{
		ms_instance = new Renderer(p_screenWidth, p_screenHeight);
	}
}

Renderer* Renderer::getInstance()
{
	if (ms_instance == NULL)
	{
		createInstance();
	}
	return ms_instance;
}

void Renderer::destroyInstance()
{
	delete ms_instance;
	ms_instance = NULL;
}

void Renderer::defaultSettings2D()
{
	m_clearBits |= GL_COLOR_BUFFER_BIT;
	m_newClearBits |= GL_COLOR_BUFFER_BIT;
	setClearColor(0, 0, 0, 1);
	useDepthTest(false);
	useStencilTest(false);
	useFaceCulling(true);

	setBlendMode(BlendMode::AlphaBlend);

	setGPUProgram(m_default2DProgram);
}

void Renderer::defaultSettings3D()
{
	m_clearBits |= GL_COLOR_BUFFER_BIT;
	m_newClearBits |= GL_COLOR_BUFFER_BIT;
	setClearColor(0, 0, 0, 1);
	useDepthTest(true);
	useStencilTest(false);
	useFaceCulling(true);

	setBlendMode(BlendMode::AlphaBlend);

	setGPUProgram(m_default3DProgram);
}

void Renderer::setBlendMode(BlendMode::Enum p_blendMode)
{
	if (p_blendMode == m_blendMode)
	{
		return;
	}
	m_blendMode = p_blendMode;

	if (p_blendMode == BlendMode::NoBlend)
	{
		glDisable(GL_BLEND);
		return;
	}

	glEnable(GL_BLEND);
	if (p_blendMode == BlendMode::AlphaBlend)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (p_blendMode == BlendMode::Additive)
	{
		glBlendFunc(GL_ONE, GL_ONE);
	}
	else if (p_blendMode == BlendMode::Multiply)
	{
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
	}

}

void Renderer::setStencilTest(Comparison::Enum p_comparison, int p_value, unsigned int p_mask)
{
	glStencilFunc(p_comparison, p_value, p_mask);
}

void Renderer::setStencilOperation(StencilOperation::Enum p_fail, StencilOperation::Enum p_pass)
{
	glStencilOp(p_fail, StencilOperation::Keep, p_pass);
}

void Renderer::setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::useDepthTest(bool p_useDepthBuffer)
{
	if (p_useDepthBuffer)
	{
		m_clearBits |= GL_DEPTH_BUFFER_BIT;
		m_newClearBits |= GL_DEPTH_BUFFER_BIT;
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		m_newClearBits &= ~GL_DEPTH_BUFFER_BIT;
		glDisable(GL_DEPTH_TEST);
	}
}

void Renderer::useStencilTest(bool p_useStencilTest)
{
	if (p_useStencilTest)
	{
		m_clearBits |= GL_STENCIL_BUFFER_BIT;
		m_newClearBits |= GL_STENCIL_BUFFER_BIT;
		glEnable(GL_STENCIL_TEST);
	}
	else
	{
		m_newClearBits &= ~GL_STENCIL_BUFFER_BIT;
		glDisable(GL_STENCIL_TEST);
	}
}

void Renderer::useFaceCulling(bool p_useFaceCulling)
{
	if (p_useFaceCulling)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void Renderer::setCullingFrontFace(CullingFrontFace::Enum p_cullingFrontFace)
{
	glFrontFace(p_cullingFrontFace);
}

void Renderer::setCullingFace(CullingFace::Enum p_cullingFace)
{
	glCullFace(p_cullingFace);
}

void Renderer::useAlphaTest(bool p_useAlhpaTest)
{
	if (p_useAlhpaTest)
	{
		glEnable(GL_ALPHA_TEST);
	}
	else
	{
		glDisable(GL_ALPHA_TEST);
	}
}

void Renderer::setTextureRenderTarget(const Texture* p_texture, bool p_useDepthBuffer)
{
	if (p_texture)
	{
		// Create framebuffer on the first call.
		if (!m_offscreenFrameBuffer)
		{
			glGenFramebuffers(1, &m_offscreenFrameBuffer);
		}

		// Use the framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_offscreenFrameBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, p_texture->getTextureId(), 0);

		// Did everything go ok?
		ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Failed to create FBO.");

		if (p_useDepthBuffer)
		{
			// Create depth buffer if it's not created yet.
			if (!m_offscreenRenderBuffer)
			{
				glGenRenderbuffers(1, &m_offscreenRenderBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_offscreenRenderBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, p_texture->getWidth(), p_texture->getHeight());
			}
			else
			{
				glBindRenderbuffer(GL_RENDERBUFFER, m_offscreenRenderBuffer);
			}

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_offscreenRenderBuffer);
		}

		// Clear color and depth buffer.
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}
	else
	{
		// Unbind the offscreen buffers.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

void Renderer::update(float p_timePassed)
{
	if (m_worldCamera)
	{
		m_worldCamera->update(p_timePassed);
	}
	m_hudCamera.update(p_timePassed);
}

void Renderer::clearColor() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clearDepth() const
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::clearStencil() const
{
	glClear(GL_STENCIL_BUFFER_BIT);
}

void Renderer::beginFrame()
{
	glClear(m_clearBits);

	m_clearBits = m_newClearBits;

	if (m_worldCamera)
	{
		setCameraMatrices(m_worldCamera);
	}
}

void Renderer::endFrame()
{
	glutSwapBuffers();
#ifndef _RELEASE
	doGraphicsErrorCheck();
#endif
}

void Renderer::setTexture(const Texture* p_texture)
{
	if (p_texture)
		glBindTexture(GL_TEXTURE_2D, p_texture->getTextureId());
	else
		m_emptyTexture->select();
}

void Renderer::setGPUProgram(const GPUProgram* p_program)
{
	if (!p_program)
		p_program = m_default2DProgram;

	if (p_program != m_currentSelectedProgram)
	{
		glUseProgram(p_program->getProgramId());
		m_currentSelectedProgram = p_program;
	}
}

const GPUProgram* Renderer::getGPUProgram() const
{
	return m_currentSelectedProgram;
}

void Renderer::renderScene() const
{
	if (m_scene)
		m_scene->render();
}

void Renderer::render(const SceneItem& p_sceneItem)
{
	if (p_sceneItem.m_indices.empty())
		return;

	m_worldMatrix.setTransformation(p_sceneItem.m_position, p_sceneItem.m_rotation, p_sceneItem.m_scale);

	render(reinterpret_cast<const float*>(&p_sceneItem.m_positions[0]), 
					 NULL, 
					 reinterpret_cast<const float*>(&p_sceneItem.m_colors[0]), 
					 reinterpret_cast<const float*>(&p_sceneItem.m_normals[0]), 
					 reinterpret_cast<const unsigned int*>(&p_sceneItem.m_indices[0]), 
					 p_sceneItem.m_indices.size(), true, false);
}

void Renderer::render(const float* p_positions, const float* p_textureCoordinates,
					  const float* p_colors, const float* p_normals,
					  const unsigned int* p_indices, unsigned int p_vertexCount,
					  bool p_3DCoordinates, bool p_triangleStrip)
{
	// Set the vertexdata and matrices. Code depending on usage of shaders.
	ASSERT(m_currentSelectedProgram, "Rendering without a shader program.");

	updateWorldViewProjectionMatrix();

	unsigned int renderMode = m_currentSelectedProgram->getRegisteredAttributes();

	if (renderMode != m_renderMode)
	{
		for (int i = 0; i < m_maxAttributes; ++i)
		{
			unsigned int attributeBit = 1 << i;
			if (attributeBit & renderMode)
				glEnableVertexAttribArray(i);
			else if (attributeBit & m_renderMode)
				glDisableVertexAttribArray(i);
		}

		m_renderMode = renderMode;
	}

	if (p_positions && m_currentSelectedProgram->getPositionLocation() != -1)
		glVertexAttribPointer(m_currentSelectedProgram->getPositionLocation(), p_3DCoordinates ? 3 : 2, GL_FLOAT, false, 0, p_positions);
	if (p_textureCoordinates && m_currentSelectedProgram->getTextureCoordintateLocation() != -1)
		glVertexAttribPointer(m_currentSelectedProgram->getTextureCoordintateLocation(), 2, GL_FLOAT, false, 0, p_textureCoordinates);
	if (p_colors && m_currentSelectedProgram->getColorLocation() != -1)
		glVertexAttribPointer(m_currentSelectedProgram->getColorLocation(), 4, GL_FLOAT, false, 0, p_colors);
	if (p_normals && m_currentSelectedProgram->getNormalLcoation() != -1)
		glVertexAttribPointer(m_currentSelectedProgram->getNormalLcoation(), 3, GL_FLOAT, false, 0, p_normals);

	// Drawing.
	if (p_indices)
		glDrawElements(GL_TRIANGLES, p_vertexCount, GL_UNSIGNED_INT, p_indices);
	else
		glDrawArrays(p_triangleStrip ? GL_TRIANGLE_STRIP : GL_TRIANGLES, 0, p_vertexCount);
}

void Renderer::startHudRendering()
{
	setCameraMatrices(&m_hudCamera);
}

void Renderer::doGraphicsErrorCheck() const
{
	for (;;)
	{
		GLenum error = glGetError();
		if (error == GL_NO_ERROR)
		{
			break;
		}

		std::string message("OpenGL error detected.\n");
		switch (error)
		{
		case GL_INVALID_ENUM:
			message += "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_INVALID_VALUE:
			message += "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_INVALID_OPERATION:
			message += "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_STACK_OVERFLOW:
			message += "This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_STACK_UNDERFLOW:
			message += "This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_OUT_OF_MEMORY:
			message += "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
			break;
		}
		PANIC(message);
	}
}

Renderer::Renderer(int p_screenWidth, int p_screenHeight):
	m_scene(NULL),
	m_worldCamera(NULL),
	m_hudCamera(Vector3(0, 0, 1), Vector3(), Math::HALF_PI),
	m_worldMatrix(Matrix44::IDENTITY),
	m_screenWidth(p_screenWidth),
	m_screenHeight(p_screenHeight),
	m_renderMode(0),
	m_maxAttributes(0),
	m_blendMode(BlendMode::NoBlend),
	m_clearBits(0),
	m_newClearBits(0),
	m_offscreenFrameBuffer(0),
	m_offscreenRenderBuffer(0),
	m_currentSelectedProgram(NULL),
	m_default2DProgram(NULL),
	m_default3DProgram(NULL),
	m_emptyTexture(NULL)
{
	initGLExtensions();

	m_hudCamera.setProjectionType(Camera::ProjectionType::Orthographic);
	setWorldCamera(&m_hudCamera);

	m_default2DProgram = new GPUProgram(false);
	m_default3DProgram = new GPUProgram(true);
	m_emptyTexture = new Texture(64, 64, 255);
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_maxAttributes);
	m_maxAttributes = Math::minimum(m_maxAttributes, (int)sizeof(unsigned int) * 8);

	defaultSettings2D();

	glViewport(0, 0, m_screenWidth, m_screenHeight);
}

Renderer::~Renderer()
{
	glDeleteFramebuffers(1, &m_offscreenFrameBuffer);
	glDeleteRenderbuffers(1, &m_offscreenRenderBuffer);

	delete m_default2DProgram;
	delete m_default3DProgram;
	delete m_emptyTexture;
}

void Renderer::setCameraMatrices(const Camera* p_camera)
{
	ASSERT(p_camera, "No camera selected.");

	p_camera->createView(m_viewMatrix);
	p_camera->createProjection(m_projectionMatrix);
}

void Renderer::updateWorldViewProjectionMatrix() const
{
	ASSERT(m_currentSelectedProgram, "No GPU Program selected.");

	// Create worldview matrix.
	m_currentSelectedProgram->setWorldMatrix(m_worldMatrix);
	m_currentSelectedProgram->setWorldViewProjectionMatrix(m_projectionMatrix * m_viewMatrix * m_worldMatrix);
}
