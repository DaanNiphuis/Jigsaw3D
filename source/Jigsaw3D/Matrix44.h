#ifndef MATRIX44_H
#define MATRIX44_H

#include "Debug.h"
#include "Vector3.h"
#include "Vector4.h"

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

	inline Vector4 operator * (const Vector4& p_vector) const
	{
		return Vector4(
			m[0][0] * p_vector.x + m[0][1] * p_vector.y + m[0][2] * p_vector.z + m[0][3] * p_vector.w, 
			m[1][0] * p_vector.x + m[1][1] * p_vector.y + m[1][2] * p_vector.z + m[1][3] * p_vector.w,
			m[2][0] * p_vector.x + m[2][1] * p_vector.y + m[2][2] * p_vector.z + m[2][3] * p_vector.w,
			m[3][0] * p_vector.x + m[3][1] * p_vector.y + m[3][2] * p_vector.z + m[3][3] * p_vector.w);
	}

	inline void setTransform(const Vector3& p_position, const Vector3& p_rotation, const Vector3& p_scale)
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

	Matrix44 Matrix44::getInverse() const
	{
		float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
		float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
		float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
		float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

		float v0 = m20 * m31 - m21 * m30;
		float v1 = m20 * m32 - m22 * m30;
		float v2 = m20 * m33 - m23 * m30;
		float v3 = m21 * m32 - m22 * m31;
		float v4 = m21 * m33 - m23 * m31;
		float v5 = m22 * m33 - m23 * m32;

		float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
		float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
		float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
		float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

		float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

		float d00 = t00 * invDet;
		float d10 = t10 * invDet;
		float d20 = t20 * invDet;
		float d30 = t30 * invDet;

		float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;

		float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;

		float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		return Matrix44(
			d00, d01, d02, d03,
			d10, d11, d12, d13,
			d20, d21, d22, d23,
			d30, d31, d32, d33);
	}

	friend std::ostream& operator << (std::ostream& p_os, const Matrix44& p_mat)
	{
		return p_os	<< "| " << p_mat[0][0] << " " << p_mat[0][1] << " " << p_mat[0][2] << " " << p_mat[0][3] << " |" << std::endl
					<< "| " << p_mat[1][0] << " " << p_mat[1][1] << " " << p_mat[1][2] << " " << p_mat[1][3] << " |" << std::endl
					<< "| " << p_mat[2][0] << " " << p_mat[2][1] << " " << p_mat[2][2] << " " << p_mat[2][3] << " |" << std::endl
					<< "| " << p_mat[3][0] << " " << p_mat[3][1] << " " << p_mat[3][2] << " " << p_mat[3][3] << " |";
	}

	static const Matrix44 IDENTITY;

};


#endif
