#ifndef VECTOR4_H
#define VECTOR4_H

#include "Debug.h"

#include <iostream>

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

	Vector4(float x, float y, float z, float w) :
		x(x),
		y(y),
		z(z),
		w(w)
	{
	}
	
	friend std::ostream& operator << (std::ostream& os, const Vector4& p_vector)
	{
		return os << "x: " << p_vector.x << " y: " << p_vector.y << " z: " << p_vector.z << " w: " << p_vector.w;
	}

};

#endif