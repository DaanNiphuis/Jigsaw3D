#include "GPUProgram.h"

#include "Debug.h"
#include "IOFunctions.h"
#include "Matrix44.h"
#include "OpenGL.h"
#include "Renderer.h"
#include "Vector3.h"

#include <string>

GPUProgram::GPUProgram(bool p_3d):
	m_vertexShader(0),
	m_pixelShader(0),
	m_program(0),
	m_worldViewProjectionMatrixLocation(0),
	m_registeredAttributes(0)
{
	if (p_3d)
	{
		create(ms_default3DVertexShaderSource, ms_default3DFragmentShaderSource);
	}
	else
	{
		create(ms_default2DVertexShaderSource, ms_default2DFragmentShaderSource);
	}
}

GPUProgram::GPUProgram(const char* p_vertexShaderFilename, const char* p_pixelShaderFilename):
	m_vertexShader(0),
	m_pixelShader(0),
	m_program(0),
	m_worldViewProjectionMatrixLocation(0),
	m_registeredAttributes(0)
{
	std::string vertexShaderFilename(p_vertexShaderFilename);
	std::string pixelShaderFilename(p_pixelShaderFilename);

	const char* source1 = NULL;
	std::string source1s;
	if (vertexShaderFilename == "")
	{
		source1 = ms_default2DVertexShaderSource;
	}
	else
	{
		getFileAsString(source1s, vertexShaderFilename.c_str());
		source1 = source1s.c_str();
	}

	const char* source2 = NULL;
	std::string source2s;
	if (pixelShaderFilename == "")
	{
		source2 = ms_default2DFragmentShaderSource;
	}
	else
	{	
		getFileAsString(source2s, pixelShaderFilename.c_str());
		source2 = source2s.c_str();
	}

#if defined _DEBUG
	std::string sourceVS(source1);
	sourceVS = "#pragma optimize(off)\n" + sourceVS;
	source1 = sourceVS.c_str();
	std::string sourceFS(source2);
	sourceFS = "#pragma optimize(off)\n" + sourceFS;
	source2 = sourceFS.c_str();
#endif

	// Turning off shader compiling optimization to prevent linking errors for 
	// unused variables for example.
	create(source1, source2);
}

GPUProgram::~GPUProgram()
{
	if (m_vertexShader)
	{
		glDetachShader(m_program, m_vertexShader);
		glDeleteShader(m_vertexShader);
	}
	if (m_pixelShader)
	{
		glDetachShader(m_program, m_pixelShader);
		glDeleteShader(m_pixelShader);
	}

	glDeleteProgram(m_program);
}

void GPUProgram::select() const
{
	Renderer::getInstance()->setGPUProgram(this);
}

int GPUProgram::getAttributeLocation(const char* p_variableName) const
{
	int location = glGetAttribLocation(m_program, p_variableName);
	registerAttribute(location);
	return location;
}

int GPUProgram::getUniformLocation(const char* p_variableName) const
{
	return glGetUniformLocation(m_program, p_variableName);
}

void GPUProgram::setUniformVariable(int p_location, int p_data) const
{
	if (p_location >= 0)
		glUniform1i(p_location, p_data);
}

void GPUProgram::setUniformVariable(int p_location, float p_data) const
{
	if (p_location >= 0)
		glUniform1f(p_location, p_data);
}

void GPUProgram::setUniformVariable(int p_location, const Vector3& p_data) const
{
	if (p_location >= 0)
		glUniform3f(p_location, p_data.x, p_data.y, p_data.z);
}

void GPUProgram::setUniformVariable(int p_location, const Matrix44& p_data) const
{
	if (p_location >= 0)
		glUniformMatrix4fv(p_location, 1, false, reinterpret_cast<const float*>(&p_data));
}

void GPUProgram::setUniformVariable(const char* p_variableName, int p_data) const
{
	glUniform1i(getUniformLocation(p_variableName), p_data);
}

void GPUProgram::setUniformVariable(const char* p_variableName, float p_data) const
{
	glUniform1f(getUniformLocation(p_variableName), p_data);
}

void GPUProgram::setUniformVariable(const char* p_variableName, const Vector3& p_data) const
{
	glUniform3f(getUniformLocation(p_variableName), p_data.x, p_data.y, p_data.z);
}

void GPUProgram::setUniformVariable(const char* p_variableName, const Matrix44& p_data) const
{
	glUniformMatrix4fv(getUniformLocation(p_variableName), 1, false, reinterpret_cast<const float*>(&p_data));
}

void GPUProgram::setAttributeData(const char* p_variableName, const float* p_data, unsigned int p_valuesPerAttribute) const
{
	setAttributeData(getAttributeLocation(p_variableName), p_data, p_valuesPerAttribute);
}

void GPUProgram::setAttributeData(int p_location, const float* p_data, unsigned int p_valuesPerAttribute) const
{	
	if (p_location >= 0)
	{
		glVertexAttribPointer(p_location, p_valuesPerAttribute, GL_FLOAT, GL_FALSE, 0, p_data);
		glEnableVertexAttribArray(p_location);
	}
}

void GPUProgram::setWorldViewProjectionMatrix(const Matrix44& p_matrix) const
{
	if (m_worldViewProjectionMatrixLocation >= 0)
		setUniformVariable(m_worldViewProjectionMatrixLocation, p_matrix.getTranspose());
}

void GPUProgram::setWorldMatrix(const Matrix44& p_matrix) const
{
	if (m_worldMatrixLocation >= 0)
		setUniformVariable(m_worldMatrixLocation, p_matrix.getTranspose());
}

void GPUProgram::printShaderInfoLog(unsigned int obj)
{
	int infologLength = 0;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		char* infoLog = new char[infologLength];
		int charsWritten = 0;
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		PRINT(infoLog);
		delete[] infoLog;
	}
}

void GPUProgram::printProgramInfoLog(unsigned int obj)
{
	int infologLength = 0;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		char* infoLog = new char[infologLength];
		int charsWritten = 0;
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		PRINT(infoLog);
		delete[] infoLog;
	}
}

void GPUProgram::create(const char* p_vsSource, const char* p_fsSource)
{
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertexShader, 1, &(p_vsSource), 0);
	glCompileShader(m_vertexShader);
	// Show compile errors.
#if defined(_DEBUG)
	int vertexShaderStatus;
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &vertexShaderStatus);
	if (vertexShaderStatus == GL_FALSE)
	{
		printShaderInfoLog(m_vertexShader);
	}
#endif

	m_pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_pixelShader, 1, &(p_fsSource), 0);
	glCompileShader(m_pixelShader);
	// Show compile errors.
#if defined(_DEBUG)
	int pixelShaderStatus;
	glGetShaderiv(m_pixelShader, GL_COMPILE_STATUS, &pixelShaderStatus);
	if (pixelShaderStatus == GL_FALSE)
	{
		printShaderInfoLog(m_pixelShader);
	}
#endif

	// We need both of the shaders otherwise this program has no use.
	ASSERT(m_vertexShader != 0 && m_pixelShader != 0, "Creating a GPUProgram without both shaders.");

	// Create the program and attach the shaders.
	m_program = glCreateProgram();
	if (m_vertexShader)
	{
		glAttachShader(m_program, m_vertexShader);
	}
	if (m_pixelShader)
	{
		glAttachShader(m_program, m_pixelShader);
	}

	glLinkProgram(m_program);
#if defined(_DEBUG)
	// Show link errors.
	int linkStatus;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE)
	{
		printProgramInfoLog(m_program);
	}
#endif

	m_positionLocation = glGetAttribLocation(m_program, "position");
	m_colorLocation = glGetAttribLocation(m_program, "color");
	m_textureCoordinateLocation = glGetAttribLocation(m_program, "textureCoordinate");
	m_normalLocation = glGetAttribLocation(m_program, "normal");

	registerAttribute(m_positionLocation);
	registerAttribute(m_colorLocation);
	registerAttribute(m_textureCoordinateLocation);
	registerAttribute(m_normalLocation);

	m_worldViewProjectionMatrixLocation = glGetUniformLocation(m_program, "worldViewProjectionMatrix");
	m_worldMatrixLocation = glGetUniformLocation(m_program, "worldMatrix");
}

void GPUProgram::registerAttribute(int location) const
{
	if (location >= 0)
	{
		m_registeredAttributes |= 1 << location;
	}
}

const char* GPUProgram::ms_default2DVertexShaderSource = 
"attribute vec4 position;"
"attribute vec4 color;"
"attribute vec2 textureCoordinate;"

"uniform mat4 worldViewProjectionMatrix;"

"varying vec4 colorVarying;"
"varying vec2 textureCoordinateVarying;"

"void main()"
"{"
"	gl_Position = worldViewProjectionMatrix * position;"
"	colorVarying = color;"
"	textureCoordinateVarying = textureCoordinate;"
"}";

const char* GPUProgram::ms_default2DFragmentShaderSource = 
"#ifdef GL_ES\n"
"precision highp float;\n"
"#endif\n"

"uniform sampler2D tex;"

"varying vec4 colorVarying;"
"varying vec2 textureCoordinateVarying;"

"void main()"
"{"
"	gl_FragColor = texture2D(tex, textureCoordinateVarying) * colorVarying;"
"}";

const char* GPUProgram::ms_default3DVertexShaderSource = 
"attribute vec4 position;"
"attribute vec2 textureCoordinate;"
"attribute vec3 normal;"

"uniform mat4 worldViewProjectionMatrix;"

"varying vec2 textureCoordinateVarying;"
"varying vec3 normalVarying;"

"void main()"
"{"
"	gl_Position = worldViewProjectionMatrix * position;"
"	textureCoordinateVarying = textureCoordinate;"
"	normalVarying = normal;"
"}";

const char* GPUProgram::ms_default3DFragmentShaderSource = 
"#ifdef GL_ES\n"
"precision highp float;\n"
"#endif\n"
	
"uniform sampler2D tex;"

"varying vec2 textureCoordinateVarying;"
"varying vec3 normalVarying;"

"void main()"
"{"
"	gl_FragColor = texture2D(tex, textureCoordinateVarying);"
"}";
