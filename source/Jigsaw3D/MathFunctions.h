#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H

#define FAST_SQUAREROOT 1

#include "Debug.h"
#include "MathConstants.h"

#include <cmath>
#include <cstdlib> 
#include <ctime> 

namespace Math
{

template <typename T> inline T absolute(T p_value);
template <typename T> inline T minimum(T p_value1, T p_value2);
template <typename T> inline T minimumAbs(T p_value1, T p_value2);
template <typename T> inline T maximum(T p_value1, T p_value2);
template <typename T> inline T maximumAbs(T p_value1, T p_value2);
inline bool		isEqual(float p_value1, float p_value2, float p_maxDifference = FLOAT_EQUAL);
inline float	clamp(float p_value, float p_min = 0.0f, float p_max = 1.0f);
template <typename T> inline T lerp(const T& p_value1, const T& p_value2, float p_progress);
template <typename T> inline T smoothInterpolation(const T& p_value1, const T& p_value2, float p_progress);
template <typename T> inline T smoothOutInterpolation(const T& p_value1, const T& p_value2, float p_progress);
template <typename T> inline T smoothInInterpolation(const T& p_value1, const T& p_value2, float p_progress);
template <typename T> inline T interpolate(const T& p_position1, const T& p_velocity1, const T& p_position2, float p_progress);
inline float square(float p_value);
inline float cube(float p_value);
inline float	power(float p_base, float p_exponent);
// Only works for int, unsigned int, etc.
template <typename T> inline bool isPowerOfTwo(T p_value);
inline unsigned int getNextPowerOfTwo(unsigned int p_value);
float			squareRoot(float p_value);
inline float	sine(float p_value);
inline float	cosine(float p_value);
inline float	tangent(float p_value);
inline float	arcSine(float p_value);
inline float	arcSine(float p_value);
inline float	arcCosine(float p_value);
inline float	arcTangent(float p_value);
inline float	arcTangent2(float p_value1, float p_value2);
inline float	radiansToDegrees(float p_radians);
inline float	degreesToRadians(float p_degrees);
inline void		initRandom();
inline void		initRandom(unsigned int p_seed);
inline int		random();
inline float	randomFloat();
inline bool		randomBool();
// int/unsigned int version returns random value between parameters but 
// can also return a value same as one of the parameters.
template <typename T> inline T randomBetween(T p_value1, T p_value2);
// Specialization for floats.
template <> inline float randomBetween(float p_value1, float p_value2);
float			randomSign();
inline float	pythagoras(float p_side1, float p_side2);
inline float	sign(float p_value);
inline float	gaussian(float p_x, float p_sigma);

template <typename T>
T absolute(T p_value)
{
	return p_value < 0 ? -p_value : p_value;
}

template <typename T>
T minimum(T p_value1, T p_value2)
{
	return p_value1 < p_value2 ? p_value1 : p_value2;
}

template <typename T> 
T minimumAbs(T p_value1, T p_value2)
{
	return absolute(p_value1) < absolute(p_value2) ? p_value1 : p_value2;
}

template <typename T>
T maximum(T p_value1, T p_value2)
{
	return p_value1 > p_value2 ? p_value1 : p_value2;
}

template <typename T>
T maximumAbs(T p_value1, T p_value2)
{
	return absolute(p_value1) > absolute(p_value2) ? p_value1 : p_value2;
}

bool isEqual(float p_value1, float p_value2, float p_maxDifference)
{
	return absolute(p_value1 - p_value2) <= p_maxDifference;
}

float clamp(float p_value, float p_min, float p_max)
{
	if (p_value < p_min)
	{
		return p_min;
	}
	if (p_value > p_max)
	{
		return p_max;
	}
	return p_value;
}

template <typename T>
T lerp(const T& p_value1, const T& p_value2, float p_progress)
{
	return p_value1 * (1.0f - p_progress) + (p_value2 * p_progress);
}

template <typename T>
T smoothInterpolation(const T& p_value1, const T& p_value2, float p_progress)
{
	if (p_progress > 0 && p_progress < 0.5f)
	{
		return smoothInInterpolation(p_value1, 
									 p_value1 * 0.5f + p_value2 * 0.5f, 
									 p_progress * 2.0f);
	}
	else if (p_progress >= 0.5f && p_progress < 1.0f)
	{
		return smoothOutInterpolation(p_value1 * 0.5f + p_value2 * 0.5f, 
									  p_value2, 
									  (p_progress - 0.5f) * 2.0f);
	}
	else
	{
		return lerp(p_value1, p_value2, p_progress);
	}
}

template <typename T>
T smoothOutInterpolation(const T& p_value1, const T& p_value2, float p_progress)
{
	if (p_progress <= 0)
	{
		return lerp(p_value1, p_value2, p_progress);
	}
	else if (p_progress < 1)
	{
		return p_value1 + (p_value2 - p_value1) * (cosine((1 - p_progress) * HALF_PI));
	}
	else
	{
		return p_value2;
	}
}

template <typename T>
T smoothInInterpolation(const T& p_value1, const T& p_value2, float p_progress)
{
	if (p_progress <= 0)
	{
		return p_value1;
	}
	else if (p_progress < 1)
	{
		return p_value1 + (p_value2 - p_value1) * (1.0f - cosine(p_progress * HALF_PI));
	}
	else
	{
		return lerp(p_value1, p_value2, p_progress);
	}
}

template <typename T> 
T interpolate(const T& p_position1, const T& p_velocity1, const T& p_position2, float p_progress)
{
	return p_position1 + p_velocity1 * p_progress + (p_position2 - p_position1 - p_velocity1) * square(p_progress);
}

template <typename T>
T interpolate(const T& p_position1, const T& p_velocity1, const T& p_position2, const T& p_velocity2, float p_progress)
{
	return p_position1 + 
		   p_velocity1 * p_progress + 
		   (-p_position1 * 3 - p_velocity1 * 2 + p_position2 * 3 - p_velocity2) * square(p_progress) + 
		   (p_position1 * 2 + p_velocity1 - p_position2*2 + p_velocity2) * cube(p_progress);
}

float square(float p_value)
{
	return p_value * p_value;
}

float cube(float p_value)
{
	return p_value * p_value * p_value;
}

float power(float p_base, float p_exponent)
{
	return std::pow(p_base, p_exponent);
}

template <typename T>
bool isPowerOfTwo(T p_value)
{
	return (p_value & (p_value - 1)) == 0;
}

unsigned int getNextPowerOfTwo(unsigned int p_value)
{
	unsigned int i = 1;

	while (i < p_value)
	{
		i *= 2;
	}
	
	return i;
}

float sine(float p_value)
{
	return sinf(p_value);
}

float cosine(float p_value)
{
	return cosf(p_value);
}

float tangent(float p_value)
{
	return tan(p_value);
}

float arcSine(float p_value)
{
	return asin(p_value);
}

float arcCosine(float p_value)
{
	return acos(p_value);
}

float arcTangent(float p_value)
{
	return atan(p_value);
}

float arcTangent2(float p_value1, float p_value2)
{
	return atan2(p_value1, p_value2);
}

float radiansToDegrees(float p_radians)
{
	return p_radians * PI_180_INV;
}

float degreesToRadians(float p_degrees)
{
	return p_degrees * PI_180;
}

void initRandom()
{
	srand(static_cast<unsigned int>(time(0)));
}

void initRandom(unsigned int p_seed)
{
	srand(p_seed);
}

int random()
{
	return rand();
}

float randomFloat()
{
	return static_cast<float>(rand()) / RAND_MAX;
}

bool randomBool()
{
	return randomFloat() < 0.5f;
}

template <typename T>
T randomBetween(T p_value1, T p_value2)
{
	ASSERT(p_value1 <= p_value2, "Value 2 smaller then Value 1.");

	return p_value1 + (random() % (p_value2 - p_value1 + 1));
}

template <>
float randomBetween<float>(float p_value1, float p_value2)
{
	ASSERT(p_value1 <= p_value2, "Value 2 smaller then Value 1.");

	return p_value1 + (p_value2 - p_value1) * randomFloat();
}

float pythagoras(float p_side1, float p_side2)
{
	return squareRoot(p_side1 * p_side1 + p_side2 * p_side2);
}

float sign(float p_value)
{
	if (p_value > 0.0f)
	{
		return 1.0f;
	}
	if (p_value < 0.0f)
	{
		return -1.0f;
	}
	return 0.0f;
}

float gaussian(float p_x, float p_sigma)
{
	return (1.0f / (squareRoot(TWO_PI) * p_sigma)) * power(E, -square(p_x) / (2.0f * square(p_sigma)));
}

}

#endif
