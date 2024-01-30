#include "glmd.hpp"

Vec2& Vec2::operator=(const Vec2& other) {
	s = other.s;
	t = other.t;
	return *this;
}

// Vector addition
Vec2 Vec2::operator+(const Vec2& other) const {
	return Vec2(s + other.s, t + other.t);
}

// Vector addition assignment
Vec2& Vec2::operator+=(const Vec2& other) {
	s += other.s;
	t += other.t;
	return *this;
}

// Vector subtraction
Vec2 Vec2::operator-(const Vec2& other) const {
	return Vec2(s - other.s, t - other.t);
}

// Vector subtraction assignment
Vec2& Vec2::operator-=(const Vec2& other) {
	s -= other.s;
	t -= other.t;
	return *this;
}

// Scalar division
Vec2 Vec2::operator/(float scalar) const {
	return Vec2(s / scalar, t / scalar);
}
// Scalar division assignment
Vec2& Vec2::operator/=(float scalar) {
	s /= scalar;
	t /= scalar;
	return *this;
}

// Scalar multiplication
Vec2 Vec2::operator*(float scalar) const {
	return Vec2(s * scalar, t * scalar);
}

// Scalar multiplication with doulbe
Vec2 Vec2::operator*(double scalar) const {
	return Vec2(s * scalar, t * scalar);
}

// Scalar multiplication assignment
Vec2& Vec2::operator*=(float scalar) {
	s *= scalar;
	t *= scalar;
	return *this;
}

// Scalar multiplication assignment with double
Vec2& Vec2::operator*=(double scalar) {
	s *= scalar;
	t *= scalar;
	return *this;
}

// Dot product
float Vec2::dot(const Vec2& other) const {
	return s * other.s + t * other.t;
}

// Length of the vector
float Vec2::length() const {
	return sqrt(s * s + t * t);
}

// Normalize the vector
Vec2 Vec2::normalize() const {
	return *this / length();
}

// Cross product
float Vec2::cross(const Vec2& other) const {
	return s * other.t - t * other.s;
}

// Equality operator
bool Vec2::operator==(const Vec2& other) const {
	return s == other.s && t == other.t;
}

// Inequality operator
bool Vec2::operator!=(const Vec2& other) const {
	return !(*this == other);
}

// Angle between two vectors
float Vec2::angle(const Vec2& other) const {
	return acos(dot(other) / (length() * other.length()));
}

// Projection of a vector onto another vector
Vec2 Vec2::project(const Vec2& other) const {
	return other * (dot(other) / other.dot(other));
}

// Reflection of a vector about another vector
Vec2 Vec2::reflect(const Vec2& other) const {
	return *this - project(other) * 2.0f;
}

// Rejection of a vector from another vector
Vec2 Vec2::reject(const Vec2& other) const {
	return *this - project(other);
}

// Lerp between two vectors
Vec2 Vec2::lerp(const Vec2& other, float t) const {
	return *this * (1.0f - t) + other * t;
}

// Slerp between two vectors
Vec2 Vec2::slerp(const Vec2& other, float t) const {
	float theta = angle(other);
	Vec2 part1 = *this * static_cast<float>(sin((1.0f - t) * theta));
	Vec2 part2 = other * static_cast<float>(sin(t * theta));
	float denominator = static_cast<float>(sin(theta));
	return (part1 + part2) / denominator;
}

