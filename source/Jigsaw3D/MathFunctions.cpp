#include "MathFunctions.h"

float Math::squareRoot(float p_value)
{
#if FAST_SQUAREROOT

	float x = p_value * 0.5f;
    float y = p_value;
    long i  = *(long*)&y;
	const float f = 1.5f;
    
	i  = 0x5f3759df - (i >> 1);
    y  = *(float*)&i;
    y  = y * (f - (x * y * y));
    y  = y * (f - (x * y * y));
   
	return p_value * y;

#else

	return std::sqrt(p_value);

#endif
}

float Math::randomSign()
{
	return random() % 2 == 0? -1.0f : 1.0f;
}