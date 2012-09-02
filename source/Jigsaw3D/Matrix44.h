#ifndef MATRIX44_H
#define MATRIX44_H

#include "Debug.h"
#include "Vector3.h"

#include <string>

class Matrix44
{
public:
	union 
	{
		float m[4][4];
		float _m[16];
	};
	
	inline Matrix44()
	{
	}

	inline Matrix44(
		float p_m00, float p_m01, float p_m02, float p_m03,
		float p_m10, float p_m11, float p_m12, float p_m13,
		float p_m20, float p_m21, float p_m22, float p_m23,
		float p_m30, float p_m31, float p_m32, float p_m33 )
	{
		_m[0] = p_m00;
		_m[1] = p_m01;
		_m[2] = p_m02;
		_m[3] = p_m03;
		_m[4] = p_m10;
		_m[5] = p_m11;
		_m[6] = p_m12;
		_m[7] = p_m13;
		_m[8] = p_m20;
		_m[9] = p_m21;
		_m[10] = p_m22;
		_m[11] = p_m23;
		_m[12] = p_m30;
		_m[13] = p_m31;
		_m[14] = p_m32;
		_m[15] = p_m33;
	}

	inline float* operator [] (size_t p_iRow)
	{
		ASSERT(p_iRow < 4, "Out of bounds.");
		return m[p_iRow];
	}

	inline const float *const operator [] (size_t p_iRow) const
	{
		ASSERT(p_iRow < 4, "Out of bounds.");
		return m[p_iRow];
	}

	inline Matrix44 concatenate(const Matrix44& p_m2) const
	{
		Matrix44 r;

		r.m[0][0] = m[0][0] * p_m2.m[0][0] + m[0][1] * p_m2.m[1][0] + m[0][2] * p_m2.m[2][0] + m[0][3] * p_m2.m[3][0];
		r.m[0][1] = m[0][0] * p_m2.m[0][1] + m[0][1] * p_m2.m[1][1] + m[0][2] * p_m2.m[2][1] + m[0][3] * p_m2.m[3][1];
		r.m[0][2] = m[0][0] * p_m2.m[0][2] + m[0][1] * p_m2.m[1][2] + m[0][2] * p_m2.m[2][2] + m[0][3] * p_m2.m[3][2];
		r.m[0][3] = m[0][0] * p_m2.m[0][3] + m[0][1] * p_m2.m[1][3] + m[0][2] * p_m2.m[2][3] + m[0][3] * p_m2.m[3][3];

		r.m[1][0] = m[1][0] * p_m2.m[0][0] + m[1][1] * p_m2.m[1][0] + m[1][2] * p_m2.m[2][0] + m[1][3] * p_m2.m[3][0];
		r.m[1][1] = m[1][0] * p_m2.m[0][1] + m[1][1] * p_m2.m[1][1] + m[1][2] * p_m2.m[2][1] + m[1][3] * p_m2.m[3][1];
		r.m[1][2] = m[1][0] * p_m2.m[0][2] + m[1][1] * p_m2.m[1][2] + m[1][2] * p_m2.m[2][2] + m[1][3] * p_m2.m[3][2];
		r.m[1][3] = m[1][0] * p_m2.m[0][3] + m[1][1] * p_m2.m[1][3] + m[1][2] * p_m2.m[2][3] + m[1][3] * p_m2.m[3][3];

		r.m[2][0] = m[2][0] * p_m2.m[0][0] + m[2][1] * p_m2.m[1][0] + m[2][2] * p_m2.m[2][0] + m[2][3] * p_m2.m[3][0];
		r.m[2][1] = m[2][0] * p_m2.m[0][1] + m[2][1] * p_m2.m[1][1] + m[2][2] * p_m2.m[2][1] + m[2][3] * p_m2.m[3][1];
		r.m[2][2] = m[2][0] * p_m2.m[0][2] + m[2][1] * p_m2.m[1][2] + m[2][2] * p_m2.m[2][2] + m[2][3] * p_m2.m[3][2];
		r.m[2][3] = m[2][0] * p_m2.m[0][3] + m[2][1] * p_m2.m[1][3] + m[2][2] * p_m2.m[2][3] + m[2][3] * p_m2.m[3][3];

		r.m[3][0] = m[3][0] * p_m2.m[0][0] + m[3][1] * p_m2.m[1][0] + m[3][2] * p_m2.m[2][0] + m[3][3] * p_m2.m[3][0];
		r.m[3][1] = m[3][0] * p_m2.m[0][1] + m[3][1] * p_m2.m[1][1] + m[3][2] * p_m2.m[2][1] + m[3][3] * p_m2.m[3][1];
		r.m[3][2] = m[3][0] * p_m2.m[0][2] + m[3][1] * p_m2.m[1][2] + m[3][2] * p_m2.m[2][2] + m[3][3] * p_m2.m[3][2];
		r.m[3][3] = m[3][0] * p_m2.m[0][3] + m[3][1] * p_m2.m[1][3] + m[3][2] * p_m2.m[2][3] + m[3][3] * p_m2.m[3][3];

		return r;
	}

	// Matrix concatenation using '*'.
	inline Matrix44 operator * (const Matrix44& p_matrix) const
	{
		return concatenate(p_matrix);
	}

	inline void setTransformation(const Vector3& p_position, const Vector3& p_rotation, const Vector3& p_scale)
	{
		const float cosX = Math::cosine(p_rotation.x);
		const float cosY = Math::cosine(p_rotation.y);
		const float cosZ = Math::cosine(p_rotation.z);
		const float sinX = Math::sine(p_rotation.x);
		const float sinY = Math::sine(p_rotation.y);
		const float sinZ = Math::sine(p_rotation.z);

		// row 0
		m[0][0] = cosY*cosZ * p_scale.x;
		m[0][1] = (-cosX*sinZ+sinX*sinY*cosZ) * p_scale.y;
		m[0][2] = (sinX*sinZ+cosX*sinY*cosZ) * p_scale.z;
		m[0][3] = p_position.x;
		// row 1
		m[1][0] = cosY*sinZ * p_scale.x;
		m[1][1] = (cosX*cosZ+sinX*sinY*sinZ) * p_scale.y;
		m[1][2] = (-sinX*cosZ+cosX*sinY*sinZ) * p_scale.z;
		m[1][3] = p_position.y;
		// row 2
		m[2][0] = -sinY * p_scale.x;
		m[2][1] = sinX*cosY * p_scale.y;
		m[2][2] = cosX*cosY* p_scale.z;
		m[2][3] = p_position.z;
		// row 3
		m[3][0] = 0;
		m[3][1] = 0;
		m[3][2] = 0;
		m[3][3] = 1;
	}

	inline void transpose()
	{
		std::swap(m[1][0],m[0][1]);
		std::swap(m[2][0],m[0][2]);
		std::swap(m[3][0],m[0][3]);
		std::swap(m[2][1],m[1][2]);
		std::swap(m[3][1],m[1][3]);
		std::swap(m[3][2],m[2][3]);
	}

	inline Matrix44 getTranspose() const
	{
		return Matrix44(m[0][0], m[1][0], m[2][0], m[3][0],
						m[0][1], m[1][1], m[2][1], m[3][1],
						m[0][2], m[1][2], m[2][2], m[3][2],
						m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	static const Matrix44 IDENTITY;

};


#endif
