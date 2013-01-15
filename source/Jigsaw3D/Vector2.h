#ifndef VECTOR2_H
#define VECTOR2_H

#include "Debug.h"

#include <iostream>

class Vector2
{
public:
	float x;
	float y;

	inline Vector2():
		x(0),
		y(0) {}

	inline Vector2(float p_x, float p_y):
		x(p_x),
		y(p_y) {}

	inline explicit Vector2(float p_value):
		x(p_value),
		y(p_value) {}

	inline void set(float p_x, float p_y)
	{
		x = p_x;
		y = p_y;
	}

	inline void set(float p_value)
	{
		x = p_value;
		y = p_value;
	}

	inline Vector2 operator +(const Vector2& p_vector) const
	{
		return Vector2(x + p_vector.x, y + p_vector.y);
	}

	inline Vector2& operator +=(const Vector2& p_vector)
	{
		x += p_vector.x;
		y += p_vector.y;
		
		return *this;
	}

	inline Vector2 operator +() const
	{
		return *this;
	}

	inline Vector2 operator -(const Vector2& p_vector) const
	{
		return Vector2(x - p_vector.x, y - p_vector.y);
	}

	inline Vector2& operator -=(const Vector2& p_vector)
	{
		x -= p_vector.x;
		y -= p_vector.y;

		return *this;
	} 

	inline Vector2 operator -() const
	{
		return getInverse();
	}

	inline Vector2 operator *(float p_value) const
	{
		return Vector2(x * p_value, y * p_value);
	}

	inline Vector2& operator *=(float p_value)
	{
		x *= p_value;
		y *= p_value;

		return *this;
	}

	inline Vector2 operator /(float p_value) const
	{
		ASSERT(p_value != 0.0f, "Attempting division by zero.");

		float division = 1.0f / p_value;
		return Vector2(x * division, y * division);
	} 

	inline Vector2& operator /= (float p_value)
	{
		ASSERT(p_value != 0.0f, "Attempting division by zero.");

		float division = 1.0f / p_value;
		x *= division;
		y *= division;

		return *this;
	}

	inline bool operator ==(const Vector2& p_vector) const
	{
		return x == p_vector.x && y == p_vector.y;
	}

	inline bool operator !=(const Vector2& p_vector) const
	{
		return x != p_vector.x || y != p_vector.y;
	}

	inline float dotProduct(const Vector2& p_vector) const
	{
		return x * p_vector.x + y * p_vector.y;
	}

	inline float crossProduct(const Vector2& p_vector) const
	{
		return x * p_vector.y - y * p_vector.x;
	}

	inline Vector2& normalize()
	{
		if (x == 0 && y == 0)
		{
			WARNING("Normalizing a zero vector.");
			x = 0;
			y = 0;
		}
		else
		{
			float inverseLength = 1.0f / getLength();
			x *= inverseLength;
			y *= inverseLength;
		}

		return *this;
	}

	inline Vector2 getNormalized() const
	{
		if (x == 0 && y == 0)
		{
			WARNING("Normalizing a zero vector.");
			return Vector2(0, 0);
		}

		float inverseLength = 1.0f / getLength();
		return Vector2(x * inverseLength, y * inverseLength);
	}

	inline float getLength() const 
	{
		return std::sqrt((x * x) + (y * y));
	}

	inline float getLengthSquared() const
	{
		return x * x + y * y;
	}

	inline Vector2& invert() 
	{
		x *= -1.0f;
		y *= -1.0f;

		return *this;
	}

	inline Vector2 getInverse() const
	{
		return Vector2(-x , -y);
	}

	inline Vector2& reset()
	{
		x = 0;
		y = 0;

		return *this;
	}

	inline bool isZero() const
	{
		return x == 0 && y == 0;
	}

	friend std::ostream& operator << (std::ostream& os, const Vector2& p_vector)
	{
		return os << "x: " << p_vector.x << " y: " << p_vector.y;
	}

};

#endif
