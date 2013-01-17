#include "Renderer.h"

#include "Camera.h"
#include "Debug.h"
#include "GPUProgram.h"
#include "IOFunctions.h"
#include "MathConstants.h"
#include "Scene.h"
#include "Texture.h"
#include "VertexIndexBuffer.h"

#include <string>

Renderer* Renderer::ms_instance = NULL;

void Renderer::createInstance(const char* p_name, int p_screenWidth, int p_screenHeight)
{
	if (ms_instance == NULL)
	{
		ms_instance = new Renderer(p_name, p_screenWidth, p_screenHeight);
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
	setClearColor(0, 0, 0, 0);
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
	setClearColor(0, 0, 0, 0);
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

void Renderer::setTextureRenderTarget(const Texture* p_texture, bool p_useDepthBuffer)
{
	if (p_texture)
	{
		// Create framebuffer on the first call.
		if (m_offscreenFrameBuffer == 0)
		{
			glGenFramebuffers(1, &m_offscreenFrameBuffer);
		}

		// Use the framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_offscreenFrameBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, p_texture->getId(), 0);

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
	}
	else
	{
		// Unbind the offscreen buffers.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
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
}

void Renderer::endFrame()
{
	m_renderWindow.endFrame();
#ifndef _RELEASE
	doGraphicsErrorCheck();
#endif
}

const GPUProgram* Renderer::getGPUProgram() const
{
	return m_GPUProgram;
}

void Renderer::setGPUProgram(const GPUProgram* p_program)
{
	if (!p_program)
		p_program = m_default2DProgram;

	if (p_program != m_GPUProgram)
	{
		glUseProgram(p_program->getId());
		m_GPUProgram = p_program;
	}
}

const VertexIndexBuffer* Renderer::getVertexIndexBuffer() const
{
	return m_vib;
}

void Renderer::setVertexIndexBuffer(const VertexIndexBuffer* p_vib)
{
	if (!p_vib)
	{
		// handle null pointer
	}

	if (p_vib != m_vib)
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_vib->getVerticesId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_vib->getIndicesId());
		m_vib = p_vib;
	}
}

void Renderer::setTexture(const Texture* p_texture, TextureSlot::Enum p_textureSlot)
{
	if (p_texture)
	{
		if (p_texture != m_textures[p_textureSlot])
		{
			if (p_textureSlot != m_currentTextureSlot)
			{
				glActiveTexture(GL_TEXTURE0 + p_textureSlot);
				m_currentTextureSlot = p_textureSlot;
			}

			glBindTexture(GL_TEXTURE_2D, p_texture->getId());
			m_textures[p_textureSlot] = p_texture;
		}
	}
	else
		m_emptyTexture->select();
}

void Renderer::renderScene()
{
	if (m_scene)
		m_scene->render();
}

void Renderer::render()
{
	// Set the vertexdata and matrices. Code depending on usage of shaders.
	ASSERT(m_GPUProgram, "Rendering without a shader program.");

	updateWorldViewProjectionMatrix();

	unsigned int registeredAttributes = m_GPUProgram->getRegisteredAttributes();

	if (registeredAttributes != m_registeredAttributes)
	{
		for (int i = 0; i < m_maxAttributes; ++i)
		{
			unsigned int attributeBit = 1 << i;
			if (attributeBit & registeredAttributes)
				glEnableVertexAttribArray(i);
			else if (attributeBit & m_registeredAttributes)
				glDisableVertexAttribArray(i);
		}

		m_registeredAttributes = registeredAttributes;
	}

	if (m_vib->getPositionsSize() > 0 && m_GPUProgram->getPositionLocation() != -1)
		glVertexAttribPointer(m_GPUProgram->getPositionLocation(), m_vib->getPositionSize(), GL_FLOAT, GL_FALSE, 0,  (const GLvoid*)m_vib->getPositionsOffset());
	if (m_vib->geTexCoordsSize() > 0 && m_GPUProgram->getTextureCoordintateLocation() != -1)
		glVertexAttribPointer(m_GPUProgram->getTextureCoordintateLocation(), 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)m_vib->getTexCoordsOffset());
	if (m_vib->getNormalsSize() > 0 && m_GPUProgram->getNormalLcoation() != -1)
		glVertexAttribPointer(m_GPUProgram->getNormalLcoation(), 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)m_vib->getNormalsOffset());
	if (m_vib->getColorsSize() > 0 && m_GPUProgram->getColorLocation() != -1)
		glVertexAttribPointer(m_GPUProgram->getColorLocation(), 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*) m_vib->getColorsOffset());

	// Drawing.
	if (m_vib->getIndicesCount() > 0)
		glDrawElements(m_vib->getRenderMode(), m_vib->getIndicesCount(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(m_vib->getRenderMode(), 0, m_vib->getVertexCount());
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
		case GL_OUT_OF_MEMORY:
			message += "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
			break;
		}
		PANIC(message);
	}
}

Renderer::Renderer(const char* p_name, int p_screenWidth, int p_screenHeight):
	m_renderWindow(p_name, p_screenWidth, p_screenHeight, false),
	m_scene(NULL),
	m_activeCamera(NULL),
	m_worldMatrix(Matrix44::IDENTITY),
	m_screenWidth(p_screenWidth),
	m_screenHeight(p_screenHeight),
	m_registeredAttributes(0),
	m_maxAttributes(0),
	m_blendMode(BlendMode::NoBlend),
	m_clearBits(0),
	m_newClearBits(0),
	m_offscreenFrameBuffer(0),
	m_offscreenRenderBuffer(0),
	m_GPUProgram(NULL),
	m_default2DProgram(NULL),
	m_default3DProgram(NULL),
	m_vib(NULL),
	m_emptyTexture(NULL),
	m_currentTextureSlot(TextureSlot::Texture0)
{
	initGLExtensions();

	m_default2DProgram = new GPUProgram(false);
	m_default3DProgram = new GPUProgram(true);
	// default vertex array object
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	// a white empty texture
	m_emptyTexture = new Texture(64, 64, 255);
	glBindTexture(GL_TEXTURE_2D, m_emptyTexture->getId());
	// set all selected textures to NULL
	for (unsigned int i = 0; i < TextureSlot::Count; ++i)
	{
		m_textures[i] = NULL;
	}
	// store max vertex attributes value
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_maxAttributes);
	m_maxAttributes = Math::minimum(m_maxAttributes, (int)sizeof(unsigned int) * 8);

	glViewport(0, 0, m_screenWidth, m_screenHeight);
}

Renderer::~Renderer()
{
	glDeleteFramebuffers(1, &m_offscreenFrameBuffer);
	glDeleteRenderbuffers(1, &m_offscreenRenderBuffer);

	delete m_default2DProgram;
	delete m_default3DProgram;
	glDeleteVertexArrays(1, &m_vao);
	delete m_emptyTexture;
}

void Renderer::updateWorldViewProjectionMatrix() const
{
	ASSERT(m_GPUProgram, "No GPU Program selected.");

	// Update GPU Program with current selected matrices.
	m_GPUProgram->setWorldMatrix(m_worldMatrix);
	if (m_activeCamera)
		m_GPUProgram->setWorldViewProjectionMatrix(m_worldMatrix, m_activeCamera->getViewProjectionMatrix());
	else // Use identity matrices if camera == NULL
		m_GPUProgram->setWorldViewProjectionMatrix(m_worldMatrix, Matrix44::IDENTITY);
}
