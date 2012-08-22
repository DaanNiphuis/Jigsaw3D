#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	float R;
	float G;
	float B;
	float A;

	Color();
	Color(float R, float G, float B, float A = 1);
	~Color();

	Color operator + (const Color& color) const;
	Color& operator += (const Color& color);

	Color operator * (float value) const;
	Color& operator *= (float value);

	bool operator == (const Color& color) const;

	static Color createFromBytes(unsigned char R, unsigned char G, unsigned char B, unsigned char A);

	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color PINK;
	static const Color BLACK;
	static const Color WHITE;
};

#endif

