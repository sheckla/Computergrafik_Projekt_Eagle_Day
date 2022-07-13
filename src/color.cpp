#include "color.h"
#include <assert.h>

Color::Color() : Color(0.0f, 0.0f, 0.0f)
{
}

Color::~Color() {
	//std::cout << "~Color()\n";
}

Color::Color(float r, float g, float b) :R(r), G(g), B(b)
{
}

Color Color::operator*(const Color& c) const
{
	return Color(
		this->R * c.R,
		this->G * c.G,
		this->B * c.B);
}

Color Color::operator*(const float Factor) const
{
	return *this * Color(Factor, Factor, Factor);
}

Color Color::operator+(const Color& c) const
{
	return Color(
		this->R + c.R,
		this->G + c.G,
		this->B + c.B);
}

Color& Color::operator+=(const Color& c)
{
	*this = *this + c;
	return *this;
}

std::ostream& operator<<(std::ostream& output, const Color& c) {
	output << "Color[" <<
		c.R << ", " << c.G << ", " << c.B << "]";
	return output;
}
