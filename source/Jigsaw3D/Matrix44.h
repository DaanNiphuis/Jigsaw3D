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
		m[0][0] = p_m00;
		m[0][1] = p_m01;
		m[0][2] = p_m02;
		m[0][3] = p_m03;
		m[1][0] = p_m10;
		m[1][1] = p_m11;
		m[1][2] = p_m12;
		m[1][3] = p_m13;
		m[2][0] = p_m20;
		m[2][1] = p_m21;
		m[2][2] = p_m22;
		m[2][3] = p_m23;
		m[3][0] = p_m30;
		m[3][1] = p_m31;
		m[3][2] = p_m32;
		m[3][3] = p_m33;
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
