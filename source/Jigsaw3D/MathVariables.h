#ifndef MATHVARIABLES_H
#define MATHVARIABLES_H

#include <limits>

namespace Math
{

const float MAX_FLOAT	= std::numeric_limits<float>::max();
const float MIN_FLOAT	= std::numeric_limits<float>::min();

const float FLOAT_EQUAL = 1.0e-5f;

const float PI			= 3.141592653589793f;
const float PI_INV		= 1.0f / PI;
const float HALF_PI		= PI * 0.5f;
const float HALF_PI_INV	= 1.0f / HALF_PI;
const float TWO_PI		= PI * 2.0f;
const float TWO_PI_INV	= 1.0f / TWO_PI;
const float PI_180		= PI / 180.0f;
const float PI_180_INV	= 1.0f / PI_180;

const float E			= 2.718281828459045f;

}

#endif
