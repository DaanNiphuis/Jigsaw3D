#ifndef VECTOR3_H
#define VECTOR3_H

#include "Debug.h"
#include "MathFunctions.h"

#include <iostream>

class Vector3
{
public:
	float x;
	float y;
	float z;

	inline Vector3():
		x(0),
		y(0),
		z(0) 
	{}

	inline Vector3(float p_x, float p_y, float p_z):
		x(p_x),
		y(p_y),
		z(p_z) 
	{}

	inline explicit Vector3(float p_value):
		x(p_value),
		y(p_value),
		z(p_value)
	{}

	inline ~Vector3() {}

	inline void set(float p_value)
	{
		x = p_value;
		y = p_value;
		z = p_value;
	}

	inline Vector3 operator + (const Vector3& p_vector) const
	{
		return Vector3(x + p_vector.x, y + p_vector.y, z + p_vector.z);
	}

	inline Vector3& operator += (const Vector3& p_vector)
	{
		x += p_vector.x;
		y += p_vector.y;
		z += p_vector.z;

		return *this;
	}

	inline Vector3 operator +() const
	{
		return *this;
	}

	inline Vector3 operator - (const Vector3& p_vector) const
	{
		return Vector3(x - p_vector.x, y - p_vector.y, z - p_vector.z);
	}

	inline Vector3& operator -= (const Vector3& p_vector)
	{
		x -= p_vector.x;
		y -= p_vector.y;
		z -= p_vector.z;

		return *this;
	}

	inline Vector3 operator -() const
	{
		return Vector3(-x, -y, -z);
	}

	inline Vector3 operator * (float p_float) const
	{
		return Vector3(x * p_float, y * p_float, z * p_float);
	}

	inline Vector3& operator *= (const float p_scalar)
	{
		x *= p_scalar;
		y *= p_scalar;
		z *= p_scalar;

		return *this;
	}

	inline Vector3& operator *= (const Vector3& p_vector)
	{
		x *= p_vector.x;
		y *= p_vector.y;
		z *= p_vector.z;

		return *this;
	}

	inline Vector3 operator / (float p_float) const
	{
		float division = 1.0f / p_float;
		return Vector3(x * division, y * division, z * division);
	}

	inline bool operator == (const Vector3& p_vector) const
	{
		return x == p_vector.x && y == p_vector.y && z == p_vector.z;
	}

	inline bool operator != (const Vector3& p_vector) const
	{
		return x != p_vector.x || y != p_vector.y || z != p_vector.z;
	}

	inline float dotProduct(const Vector3& p_vector) const
	{
		return x * p_vector.x + y * p_vector.y + z * p_vector.z;
	}

	inline Vector3 crossProduct(const Vector3& p_vector) const
    {
		return Vector3(
			y * p_vector.z - z * p_vector.y,
			z * p_vector.x - x * p_vector.z,
			x * p_vector.y - y * p_vector.x);
    }

	inline Vector3& rotateAroundY(float p_radians)
	{
		float cs = Math::cosine(p_radians);
		float sn = Math::sine(p_radians);
		float tempX = x * cs - z * sn;
		z = x * sn + z * cs;
		x = tempX;
		return *this;
	}

	inline Vector3& rotateAroundZ(float p_radians)
	{
		float cs = Math::cosine(p_radians);
		float sn = Math::sine(p_radians);
		float tempX = x * cs - y * sn;
		y = x * sn + y * cs;
		x = tempX;
		return *this;
	}

	inline Vector3& rotateAroundX(float p_radians)
	{
		float cs = Math::cosine(p_radians);
		float sn = Math::sine(p_radians);
		float tempY = y * cs - z * sn;
		z = y * sn + z * cs;
		y = tempY;
		return *this;
	}

	inline Vector3& normalize()
	{
		if (x == 0 && y == 0 && z == 0)
		{
			WARNING("Normalizing a zero vector.");
			x = 0;
			y = 0;
			z = 0;
		}
		else
		{
			float inverseLength = 1.0f / Math::squareRoot( x * x + y * y + z * z );

			x *= inverseLength;
			y *= inverseLength;
			z *= inverseLength;
		}

		return *this;
	}

	inline Vector3 getNormalized() const
	{
		if (x == 0 && y == 0 && z == 0)
		{
			WARNING("Normalizing a zero vector.");
			return Vector3(0,0,0);
		}

		float inverseLength = 1.0f / Math::squareRoot( x * x + y * y + z * z );

		return Vector3(x * inverseLength, y * inverseLength, z * inverseLength);
	}

	inline float getLength() const
	{
		return Math::squareRoot(x * x + y * y + z * z);
	}

	// Standard vectors.
	static const Vector3 ZERO;
	static const Vector3 UNIT_X;
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z;
	static const Vector3 UP;
	static const Vector3 MAX;

	friend std::ostream& operator << (std::ostream& os, const Vector3& p_vector)
	{
		return os << "x: " << p_vector.x << " y: " << p_vector.y << " z: " << p_vector.z;
	}

};

#endif

