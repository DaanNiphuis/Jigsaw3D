#include "Renderer.h"

#include "Debug.h"
#include "GPUProgram.h"
#include "IOFunctions.h"
#include "MathVariables.h"
#include "Texture.h"

#include <string>

Renderer* Renderer::ms_instance = 0;
const GPUProgram* Renderer::ms_currentSelectedProgram = 0;

void Renderer::createInstance(int p_screenWidth, int p_screenHeight)
{
	if (ms_instance == 0)
	{
		ms_instance = new Renderer(p_screenWidth, p_screenHeight);
	}
}

Renderer* Renderer::getInstance()
{
	if (ms_instance == 0)
	{
		createInstance();
	}
	return ms_instance;
}

void Renderer::destroyInstance()
{
	delete ms_instance;
	ms_instance = 0;
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

	if (p_blendMode == BlendMode::NoneN)
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

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							   GL_TEXTURE_2D, p_texture->getTextureId(), 0);

		if (p_useDepthBuffer)
		{
			// Create depth buffer if it's not created yet.
			if (!m_offscreenRenderBuffer)
			{
				glGenRenderbuffers(1, &m_offscreenRenderBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_offscreenRenderBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
									  static_cast<int>(p_texture->getWidth()),
									  static_cast<int>(p_texture->getHeight()));
			}
			else
			{
				glBindRenderbuffer(GL_RENDERBUFFER, m_offscreenRenderBuffer);
			}

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
									  GL_RENDERBUFFER, m_offscreenRenderBuffer);
		}

		// Did everything go ok?
		ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Failed to create FBO.");

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

void Renderer::beginFrame()
{
	glClear(m_clearBits);

	m_clearBits = m_newClearBits;

	if (m_worldCamera)
	{
		m_worldCamera->apply();
	}
}

void Renderer::endFrame()
{
	glutSwapBuffers();
#ifndef _RELEASE
	doGraphicsErrorCheck();
#endif
}

void Renderer::setCameraMatrices(const Matrix44& p_viewMatrix,
								 const Matrix44& p_projectionMatrix)
{
	m_viewMatrix = p_viewMatrix;
	m_projectionMatrix = p_projectionMatrix;
	updateModelViewProjectionMatrix();
}

void Renderer::setTexture(const Texture* p_texture)
{
	if (p_texture)
	{
		glBindTexture(GL_TEXTURE_2D, p_texture->getTextureId());
	}
	else
	{
		m_emptyTexture->select();
	}
}

void Renderer::setGPUProgram(const GPUProgram* p_program)
{
	if (!p_program)
	{
		p_program = m_default2DProgram;
	}

	if (p_program != ms_currentSelectedProgram)
	{
		glUseProgram(p_program->getProgramId());
		ms_currentSelectedProgram = p_program;
	}

	updateModelViewProjectionMatrix();
}

void Renderer::render(const float* p_positions, const float* p_textureCoordinates,
					  const float* p_colors, const float* p_normals,
					  const unsigned int* p_indices, unsigned int p_vertexCount,
					  bool p_3DCoordinates, bool p_triangleStrip)
{
	// Set the vertexdata and matrices. Code depending on usage of shaders.
	ASSERT(ms_currentSelectedProgram, "Rendering without a shader program.");
	unsigned int renderMode = ms_currentSelectedProgram->getRegisteredAttributes();

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

	if (!p_textureCoordinates)
		Texture::deselect();

	ASSERT(!p_positions || (p_positions && ms_currentSelectedProgram->getPositionLocation() != -1), "If positions are provided, the GPU shader program requires a position attribute.");
	ASSERT(!p_textureCoordinates || (p_textureCoordinates && ms_currentSelectedProgram->getTextureCoordintateLocation() != -1), "If texcoords are provided, the GPU shader program requires a textureCoordinate attribute.");
	ASSERT(!p_colors || (p_colors && ms_currentSelectedProgram->getColorLocation() != -1), "If colors are provided, the GPU shader program requires a color attribute.");
	ASSERT(!p_normals || (p_normals && ms_currentSelectedProgram->getNormalLcoation() != -1), "If normals are provided, the GPU shader program requires a normal attribute.");

	if (p_positions)
		glVertexAttribPointer(ms_currentSelectedProgram->getPositionLocation(), p_3DCoordinates ? 3 : 2, GL_FLOAT, false, 0, p_positions);
	if (p_textureCoordinates)
		glVertexAttribPointer(ms_currentSelectedProgram->getTextureCoordintateLocation(), 2, GL_FLOAT, false, 0, p_textureCoordinates);
	if (p_colors)
		glVertexAttribPointer(ms_currentSelectedProgram->getColorLocation(), 4, GL_FLOAT, false, 0, p_colors);
	if (p_normals)
		glVertexAttribPointer(ms_currentSelectedProgram->getNormalLcoation(), 3, GL_FLOAT, false, 0, p_normals);

	// Drawing.
	if (p_indices)
		glDrawElements(GL_TRIANGLES, p_vertexCount, GL_UNSIGNED_INT, p_indices);
	else
		glDrawArrays(p_triangleStrip ? GL_TRIANGLE_STRIP : GL_TRIANGLES, 0, p_vertexCount);
}

void Renderer::startHudRendering() const
{
	m_hudCamera.apply();
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
	m_worldCamera(0),
	m_hudCamera(Vector3(0, 0, 1), Vector3(), Math::HALF_PI),
	m_screenWidth(p_screenWidth),
	m_screenHeight(p_screenHeight),
	m_renderMode(0),
	m_maxAttributes(0),
	m_blendMode(BlendMode::NoneN),
	m_clearBits(0),
	m_newClearBits(0),
	m_offscreenFrameBuffer(0),
	m_offscreenRenderBuffer(0),
	m_default2DProgram(0),
	m_default3DProgram(0),
	m_emptyTexture(0)
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

void Renderer::updateModelViewProjectionMatrix()
{
	// Create modelview matrix.
	ms_currentSelectedProgram->setModelViewProjectionMatrix((m_projectionMatrix * m_viewMatrix).getTranspose());
}


