#include "Color.h"

const Color Color::RED(1, 0, 0);
const Color Color::GREEN(0, 1, 0);
const Color Color::BLUE(0, 0, 1);
const Color Color::PINK(1, 0, 1);
const Color Color::BLACK(0, 0, 0);
const Color Color::WHITE(1, 1, 1);

Color::Color():
	R(1),
	G(1),
	B(1),
	A(1)
{
}

Color::Color(float p_R, float p_G, float p_B, float p_A):
	R(p_R),
	G(p_G),
	B(p_B),
	A(p_A)
{
}

Color::~Color()
{
}

Color Color::operator + (const Color& color) const
{
	return Color(R + color.R, G + color.G, B + color.B, A + color.A);
}

Color& Color::operator += (const Color& color)
{
	R += color.R;
	G += color.G;
	B += color.B;
	A += color.A;

	return *this;
}

Color Color::operator * (float value) const
{
	return Color(R * value, G * value, B * value, A * value);
}

Color& Color::operator *= (float value)
{
	R *= value;
	G *= value;
	B *= value;
	A *= value;

	return *this;
}

bool Color::operator == (const Color& color) const
{
	return color.R == R && color.G == G && color.B == B && color.A == A;
}

Color Color::createFromBytes(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
{
	static const float factor = 1.0f / 255.0f;
	return Color(R * factor, G * factor, B * factor, A * factor);
}
