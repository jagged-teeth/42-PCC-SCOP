#ifndef GLMD_HPP
#define GLMD_HPP

#include <cmath>
#include <stdexcept>
#include <functional>

inline float radians(float degrees) {
	return degrees * static_cast<float>(M_PI) / 180.0f;
}

class Vec2{
	public:
		float s, t;

		Vec2() : s(0), t(0) {}
		Vec2(float s, float t) : s(s), t(t) {}

		// Copy constructor
		Vec2(const Vec2& other) : s(other.s), t(other.t) {}

		// Assignment operator
		Vec2& operator=(const Vec2& other) {
			s = other.s;
			t = other.t;
			return *this;
		}

		// Vector addition
		Vec2 operator+(const Vec2& other) const {
			return Vec2(s + other.s, t + other.t);
		}

		// Vector addition assignment
		Vec2& operator+=(const Vec2& other) {
			s += other.s;
			t += other.t;
			return *this;
		}

		// Vector subtraction
		Vec2 operator-(const Vec2& other) const {
			return Vec2(s - other.s, t - other.t);
		}

		// Vector subtraction assignment
		Vec2& operator-=(const Vec2& other) {
			s -= other.s;
			t -= other.t;
			return *this;
		}

		// Scalar division
		Vec2 operator/(float scalar) const {
			return Vec2(s / scalar, t / scalar);
		}
		// Scalar division assignment
		Vec2& operator/=(float scalar) {
			s /= scalar;
			t /= scalar;
			return *this;
		}

		// Scalar multiplication
		Vec2 operator*(float scalar) const {
			return Vec2(s * scalar, t * scalar);
		}

		// Scalar multiplication with doulbe
		Vec2 operator*(double scalar) const {
			return Vec2(s * scalar, t * scalar);
		}

		// Scalar multiplication assignment
		Vec2& operator*=(float scalar) {
			s *= scalar;
			t *= scalar;
			return *this;
		}

		// Scalar multiplication assignment with double
		Vec2& operator*=(double scalar) {
			s *= scalar;
			t *= scalar;
			return *this;
		}

		// Dot product
		float dot(const Vec2& other) const {
			return s * other.s + t * other.t;
		}

		// Length of the vector
		float length() const {
			return sqrt(s * s + t * t);
		}

		// Normalize the vector
		Vec2 normalize() const {
			return *this / length();
		}

		// Cross product
		float cross(const Vec2& other) const {
			return s * other.t - t * other.s;
		}

		// Equality operator
		bool operator==(const Vec2& other) const {
			return s == other.s && t == other.t;
		}

		// Inequality operator
		bool operator!=(const Vec2& other) const {
			return !(*this == other);
		}

		// Angle between two vectors
		float angle(const Vec2& other) const {
			return acos(dot(other) / (length() * other.length()));
		}

		// Projection of a vector onto another vector
		Vec2 project(const Vec2& other) const {
			return other * (dot(other) / other.dot(other));
		}

		// Reflection of a vector about another vector
		Vec2 reflect(const Vec2& other) const {
			return *this - project(other) * 2.0f;
		}

		// Rejection of a vector from another vector
		Vec2 reject(const Vec2& other) const {
			return *this - project(other);
		}

		// Lerp between two vectors
		Vec2 lerp(const Vec2& other, float t) const {
			return *this * (1.0f - t) + other * t;
		}

		// Slerp between two vectors
		Vec2 slerp(const Vec2& other, float t) const {
			float theta = angle(other);
			Vec2 part1 = *this * static_cast<float>(sin((1.0f - t) * theta));
			Vec2 part2 = other * static_cast<float>(sin(t * theta));
			float denominator = static_cast<float>(sin(theta));
			return (part1 + part2) / denominator;
		}
};

// Specialization of std::hash for Vec2
namespace std {
	template <>
	struct hash<Vec2> {
		size_t operator()(const Vec2& v) const {
			size_t h1 = hash<float>()(v.s);
			size_t h2 = hash<float>()(v.t);
			return h1 ^ (h2 << 1);
		}
	};
}

class Vec3 {
	public:
		float x, y, z;

		Vec3() : x(0), y(0), z(0) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

		// Copy constructor
		Vec3(const Vec3& other) : x(other.x), y(other.y), z(other.z) {}

		// Assignment operator
		Vec3& operator=(const Vec3& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		// Vector addition
		Vec3 operator+(const Vec3& other) const {
			return Vec3(x + other.x, y + other.y, z + other.z);
		}

		// Vector addition assignment
		Vec3& operator+=(const Vec3& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		// Vector subtraction
		Vec3 operator-(const Vec3& other) const {
			return Vec3(x - other.x, y - other.y, z - other.z);
		}

		// Vector subtraction assignment
		Vec3& operator-=(const Vec3& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		// Scalar division
		Vec3 operator/(float scalar) const {
			return Vec3(x / scalar, y / scalar, z / scalar);
		}

		// Scalar division assignment
		Vec3& operator/=(float scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		// Scalar multiplication
		Vec3 operator*(float scalar) const {
			return Vec3(x * scalar, y * scalar, z * scalar);
		}

		// Scalar multiplication with doulbe
		Vec3 operator*(double scalar) const {
			return Vec3(x * scalar, y * scalar, z * scalar);
		}

		// Scalar multiplication assignment
		Vec3& operator*=(float scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		// Scalar multiplication assignment with double
		Vec3& operator*=(double scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		// Dot product
		float dot(const Vec3& other) const {
			return x * other.x + y * other.y + z * other.z;
		}

		// Length of the vector
		float length() const {
			return sqrt(x * x + y * y + z * z);
		}

		// Normalize the vector
		Vec3 normalize() const {
			return *this / length();
		}

		// Cross product
		Vec3 cross(const Vec3& other) const {
			return Vec3(y * other.z - z * other.y,
						z * other.x - x * other.z,
						x * other.y - y * other.x);
		}

		// Equality operator
		bool operator==(const Vec3& other) const {
			return x == other.x && y == other.y && z == other.z;
		}

		// Inequality operator
		bool operator!=(const Vec3& other) const {
			return !(*this == other);
		}

		// Angle between two vectors
		float angle(const Vec3& other) const {
			return acos(dot(other) / (length() * other.length()));
		}

		// Projection of a vector onto another vector
		Vec3 project(const Vec3& other) const {
			return other * (dot(other) / other.dot(other));
		}

		// Reflection of a vector about another vector
		Vec3 reflect(const Vec3& other) const {
			return *this - project(other) * 2.0f;
		}

		// Rejection of a vector from another vector
		Vec3 reject(const Vec3& other) const {
			return *this - project(other);
		}

		// Lerp between two vectors
		Vec3 lerp(const Vec3& other, float t) const {
			return *this * (1.0f - t) + other * t;
		}

		// Slerp between two vectors
		Vec3 slerp(const Vec3& other, float t) const {
			float theta = angle(other);
			Vec3 part1 = *this * static_cast<float>(sin((1.0f - t) * theta));
			Vec3 part2 = other * static_cast<float>(sin(t * theta));
			float denominator = static_cast<float>(sin(theta));
			return (part1 + part2) / denominator;
		}

		// Read-Only access to vector components
		const float& operator[](int index) const {
				switch (index) {
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: throw std::out_of_range("Vec3 index out of range");
			}
		}

		// Writable Subscript Operator
		float& operator[](int index) {
			switch (index) {
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: throw std::out_of_range("Vec3 index out of range");
			}
		}
};

namespace std {
	template <>
	struct hash<Vec3> {
		size_t operator()(const Vec3& v) const {
			size_t h1 = hash<float>()(v.x);
			size_t h2 = hash<float>()(v.y);
			size_t h3 = hash<float>()(v.z);
			return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
		}
	};
}

class Mat4 {
public:
	float data[4][4];

	Mat4() {
		// Initialize to identity matrix
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				data[i][j] = (i == j) ? 1.0f : 0.0f;
	}

	// Constructor from 2D array
	Mat4(float matrix[4][4]) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; j++)
				data[i][j] = matrix[i][j];
	}

	// Copy constructor
	Mat4(const Mat4& other) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; j++)
				data[i][j] = other.data[i][j];
	}

	// Assignment operator
	Mat4& operator=(const Mat4& other) {
		if (this != &other) {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					data[i][j] = other.data[i][j];
		}
		return *this;
	}

	// Element access
	float* operator[](int index) {
		return data[index];
	}

	const float* operator[](int index) const {
		return data[index];
	}

	// Matrix multiplication
	Mat4 operator*(const Mat4& other) const {
		Mat4 result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.data[i][j] = 0.0f;
				for (int k = 0; k < 4; ++k) {
					result.data[i][j] += data[i][k] * other.data[k][j];
				}
			}
		}
		return result;
	}

	// Matrix transpose
	Mat4 transpose() const {
	Mat4 result;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			result.data[j][i] = data[i][j];
	return result;
	}

	// Matrix translation
	static Mat4 translate(const Vec3& v) {
	Mat4 result;
	result.data[3][0] = v.x;
	result.data[3][1] = v.y;
	result.data[3][2] = v.z;
	return result;
	}

	// Matrix rotation
	Mat4 rotate(float angle, const Vec3& axis) {
	Vec3 normalizedAxis = axis.normalize();
	float rad = radians(angle);
	float cosA = cos(rad);
	float sinA = sin(rad);
	float oneMinusCosA = 1.0f - cosA;

	float xx = normalizedAxis.x * normalizedAxis.x;
	float yy = normalizedAxis.y * normalizedAxis.y;
	float zz = normalizedAxis.z * normalizedAxis.z;
	float xy = normalizedAxis.x * normalizedAxis.y;
	float xz = normalizedAxis.x * normalizedAxis.z;
	float yz = normalizedAxis.y * normalizedAxis.z;
	float xSinA = normalizedAxis.x * sinA;
	float ySinA = normalizedAxis.y * sinA;
	float zSinA = normalizedAxis.z * sinA;

	Mat4 rotationMatrix;
	rotationMatrix[0][0] = xx * oneMinusCosA + cosA;
	rotationMatrix[0][1] = xy * oneMinusCosA - zSinA;
	rotationMatrix[0][2] = xz * oneMinusCosA + ySinA;
	rotationMatrix[1][0] = xy * oneMinusCosA + zSinA;
	rotationMatrix[1][1] = yy * oneMinusCosA + cosA;
	rotationMatrix[1][2] = yz * oneMinusCosA - xSinA;
	rotationMatrix[2][0] = xz * oneMinusCosA - ySinA;
	rotationMatrix[2][1] = yz * oneMinusCosA + xSinA;
	rotationMatrix[2][2] = zz * oneMinusCosA + cosA;

	rotationMatrix[3][3] = 1.0f;

	return rotationMatrix;
}

	// Matrix Scaling
	static Mat4 scale(const Vec3& scaleFactors) {
		Mat4 result;
		result.data[0][0] = scaleFactors.x;
		result.data[1][1] = scaleFactors.y;
		result.data[2][2] = scaleFactors.z;
		result.data[3][3] = 1.0f; // No scaling on w
		return result;
	}

	// Matrix lookAt
	static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
		Vec3 f = (center - eye).normalize(); // Forward
		Vec3 r = f.cross(up).normalize();	 // Right
		Vec3 u = r.cross(f);				 // Up

		Mat4 result;
		result[0][0] = r.x;
		result[1][0] = r.y;
		result[2][0] = r.z;
		result[0][1] = u.x;
		result[1][1] = u.y;
		result[2][1] = u.z;
		result[0][2] = -f.x;
		result[1][2] = -f.y;
		result[2][2] = -f.z;

		result[3][0] = -r.dot(eye);
		result[3][1] = -u.dot(eye);
		result[3][2] = f.dot(eye);

		result[3][3] = 1.0f;

		return result;
	}

	// Matrix perspective
	static Mat4 perspective(float fov, float aspect, float near, float far) {
		Mat4 result;

		// Calculate the scale factors
		float tanHalfFov = tan(fov / 2.0f);
		float zRange = near - far;

		result.data[0][0] = 1.0f / (tanHalfFov * aspect);
		result.data[1][1] = 1.0f / tanHalfFov;
		result.data[2][2] = (-near - far) / zRange;
		result.data[2][3] = 2.0f * far * near / zRange;
		result.data[3][2] = 1.0f;
		result.data[3][3] = 0.0f;

		return result;
	}
};

#endif
