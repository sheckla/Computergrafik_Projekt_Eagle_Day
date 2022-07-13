#include "vector.h"
#include <assert.h>
#include <math.h>

Vector::Vector(float x, float y, float z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

Vector::Vector()
{
	// keine Initialisierung
}

float Vector::dot(const Vector& v) const
{
	float sum{ 0 };
	sum += this->X * v.X;
	sum += this->Y * v.Y;
	sum += this->Z * v.Z;
	return sum;
}

Vector Vector::cross(const Vector& v) const
{
	float a1 = this->X;
	float a2 = this->Y;
	float a3 = this->Z;
	float b1 = v.X;
	float b2 = v.Y;
	float b3 = v.Z;

	float x = a2 * b3 - a3 * b2;
	float y = a3 * b1 - a1 * b3;
	float z = a1 * b2 - a2 * b1;

	return Vector(x, y, z);
}



Vector Vector::operator+(const Vector& v) const
{
	return Vector(this->X + v.X, this->Y + v.Y, this->Z + v.Z);
}

Vector Vector::operator-(const Vector& v) const
{
	return Vector(this->X - v.X, this->Y - v.Y, this->Z - v.Z);
}

Vector Vector::operator*(float c) const
{
	return Vector(this->X * c, this->Y * c, this->Z * c);
}

Vector Vector::operator-() const
{
	return Vector(-this->X, -this->Y, -this->Z);
}

Vector& Vector::operator+=(const Vector& v)
{
	this->X += v.X;
	this->Y += v.Y;
	this->Z += v.Z;
	return *this;
}

Vector& Vector::normalize()
{
	float norm = 1 / length();
	*this = *this * norm;
	return *this;
}

float Vector::length() const
{
	float length = sqrt(lengthSquared());
	return length;
}

float Vector::lengthSquared() const
{
	float length = sqrt(pow(this->X, 2) + pow(this->Y, 2) + pow(this->Z, 2));
	return pow(length, 2);
}

Vector Vector::reflection(const Vector& normal) const
{
	float normalDotTimes2 = 2 * this->dot(normal);
	Vector normalCpy = normal * normalDotTimes2;
	Vector rflct = *this - normalCpy;
	return rflct;
}

/* d - ray-direction
 * this - ray-origin
 * a,b,c - triangle-vertices
 * s - ray-length
 */
bool Vector::triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
	const float EPSILON = 1e-6;

	// normal
	Vector n = (b - a).cross(c - a).normalize();	// (b-a)X(c-a) / (||(b-a)X(c-a)||)

	// min. distance to intersection
	s = (a.dot(n) - this->dot(n)) * (1 / d.dot(n));

	// visibility test
	if (s <= 0) return false;

	Vector p = *this + (d * s);					// p = o + sd

	// Intersection within triangle
	float abc = (b - a).cross(c - a).length() * 0.5;
	float abp = (b - a).cross(p - a).length() * 0.5;
	float acp = (c - a).cross(p - a).length() * 0.5;
	float bcp = (c - b).cross(p - b).length() * 0.5;

	bool result = ((abc + EPSILON) >= (abp + acp + bcp));
	return result;
}

std::ostream& operator<<(std::ostream& output, const Vector& rhs) {
	output << "Vector:[" <<
		rhs.X << ", " << rhs.Y << ", " << rhs.Z << "] ";
	return output;
}
