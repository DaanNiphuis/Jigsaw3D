#ifndef GPUPROGRAM_H
#define GPUPROGRAM_H

#include "RendererEnums.h"
#include "Uncopyable.h"

class Matrix44;
class Vector3;

class GPUProgram: Uncopyable
{
public:
	GPUProgram(bool p_3d);
	GPUProgram(const char* p_vertexShaderFilename, const char* p_pixelShaderFilename);
	~GPUProgram();

	void select() const;

	int getAttributeLocation(const char* p_variableName) const;
	inline unsigned int getRegisteredAttributes() const {return m_registeredAttributes;}
	int getUniformLocation(const char* p_variableName) const;

	// Location based.
	void setUniform(int p_location, int p_data) const;
	void setUniform(int p_location, float p_data) const;
	void setUniform(int p_location, const Vector3& p_data) const;
	void setUniform(int p_location, const Matrix44& p_data) const;
	void setUniform(int p_location, const TextureSlot::Enum p_data) const;

	// String based.
	void setUniform(const char* p_variableName, int p_data) const;
	void setUniform(const char* p_variableName, float p_data) const;
	void setUniform(const char* p_variableName, const Vector3& p_data) const;
	void setUniform(const char* p_variableName, const Matrix44& p_data) const;
	void setUniform(const char* p_variableName, const TextureSlot::Enum p_data) const;

	// Set attribute data
	void setAttributeData(const char* p_variableName, const float* p_data, unsigned int p_valuesPerAttribute) const;
	void setAttributeData(int p_location, const float* p_data, unsigned int p_valuesPerAttribute) const;

	// Default attributes
	inline int getPositionLocation() const				{return m_positionLocation;}
	inline int getColorLocation() const					{return m_colorLocation;}
	inline int getTextureCoordintateLocation() const	{return m_textureCoordinateLocation;}
	inline int getNormalLcoation() const				{return m_normalLocation;}

	// Default matrices
	void setWorldViewProjectionMatrix(const Matrix44& p_matrix) const;
	void setWorldViewProjectionMatrix(const Matrix44& p_worldMatrix, const Matrix44& p_viewProjectMatrix) const;
	void setWorldMatrix(const Matrix44& p_matrix) const;

	inline unsigned int getId() const {return m_program;}

private:
	void create(const char* p_vsSource, const char* p_fsSource);
	void registerAttribute(int location) const;

	unsigned int m_vertexShader;
	unsigned int m_pixelShader;
	unsigned int m_program;
	int m_worldViewProjectionMatrixLocation;
	int m_worldMatrixLocation;
	int m_positionLocation;
	int m_colorLocation;
	int m_textureCoordinateLocation;
	int m_normalLocation;
	mutable unsigned int m_registeredAttributes;

	static void printShaderInfoLog(unsigned int obj);
	static void printProgramInfoLog(unsigned int obj);

	static const char* ms_default2DVertexShaderSource;
	static const char* ms_default2DFragmentShaderSource;
	static const char* ms_default3DVertexShaderSource;
	static const char* ms_default3DFragmentShaderSource;
};

#endif
