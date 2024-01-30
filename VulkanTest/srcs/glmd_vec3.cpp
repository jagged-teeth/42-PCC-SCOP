#include "glmd.hpp"

Vec3& Vec3::operator=(const Vec3& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

// Vector addition
Vec3 Vec3::operator+(const Vec3& other) const {
	return Vec3(x + other.x, y + other.y, z + other.z);
}

// Vector addition assignment
Vec3& Vec3::operator+=(const Vec3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// Vector subtraction
Vec3 Vec3::operator-(const Vec3& other) const {
	return Vec3(x - other.x, y - other.y, z - other.z);
}

// Vector Unary Negation
Vec3 Vec3::operator-() const {
	return Vec3(-x, -y, -z);
}

// Vector subtraction assignment
Vec3& Vec3::operator-=(const Vec3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

// Scalar division
Vec3 Vec3::operator/(float scalar) const {
	return Vec3(x / scalar, y / scalar, z / scalar);
}

// Scalar division assignment
Vec3& Vec3::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

// Scalar multiplication
Vec3 Vec3::operator*(float scalar) const {
	return Vec3(x * scalar, y * scalar, z * scalar);
}

// Scalar multiplication with doulbe
Vec3 Vec3::operator*(double scalar) const {
	return Vec3(x * scalar, y * scalar, z * scalar);
}

// Scalar multiplication assignment
Vec3& Vec3::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

// Scalar multiplication assignment with double
Vec3& Vec3::operator*=(double scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

// Dot product
float Vec3::dot(const Vec3& other) const {
	return x * other.x + y * other.y + z * other.z;
}

// Length of the vector
float Vec3::length() const {
	return sqrt(x * x + y * y + z * z);
}

// Normalize the vector
Vec3 Vec3::normalize() const {
	float len = length();
	if (len <= 1e-6)
		return Vec3(0.0f, 0.0f, 0.0f);
	return *this / length();
}

// Cross product
Vec3 Vec3::cross(const Vec3& other) const {
	return Vec3(y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x);
}

// Equality operator
bool Vec3::operator==(const Vec3& other) const {
	return x == other.x && y == other.y && z == other.z;
}

// Inequality operator
bool Vec3::operator!=(const Vec3& other) const {
	return !(*this == other);
}

// Angle between two vectors
float Vec3::angle(const Vec3& other) const {
	return acos(dot(other) / (length() * other.length()));
}

// Projection of a vector onto another vector
Vec3 Vec3::project(const Vec3& other) const {
	return other * (dot(other) / other.dot(other));
}

// Reflection of a vector about another vector
Vec3 Vec3::reflect(const Vec3& other) const {
	return *this - project(other) * 2.0f;
}

// Rejection of a vector from another vector
Vec3 Vec3::reject(const Vec3& other) const {
	return *this - project(other);
}

// Lerp between two vectors
Vec3 Vec3::lerp(const Vec3& other, float t) const {
	return *this * (1.0f - t) + other * t;
}

// Slerp between two vectors
Vec3 Vec3::slerp(const Vec3& other, float t) const {
	float theta = angle(other);
	Vec3 part1 = *this * static_cast<float>(sin((1.0f - t) * theta));
	Vec3 part2 = other * static_cast<float>(sin(t * theta));
	float denominator = static_cast<float>(sin(theta));
	return (part1 + part2) / denominator;
}

// Read-Only access to vector components
const float& Vec3::operator[](int index) const {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw std::out_of_range("Vec3 index out of range");
	}
}

// Writable Subscript Operator
float& Vec3::operator[](int index) {
	switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw std::out_of_range("Vec3 index out of range");
	}
}
