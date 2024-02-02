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
		Vec2(const Vec2& other) : s(other.s), t(other.t) {}

		Vec2& operator=(const Vec2& other);
		Vec2 operator+(const Vec2& other) const;
		Vec2& operator+=(const Vec2& other);
		Vec2 operator-(const Vec2& other) const;
		Vec2& operator-=(const Vec2& other);
		Vec2 operator/(float scalar) const;
		Vec2& operator/=(float scalar);
		Vec2 operator*(float scalar) const;
		Vec2 operator*(double scalar) const;
		Vec2& operator*=(float scalar);
		Vec2& operator*=(double scalar);
		float dot(const Vec2& other) const;
		float length() const;
		Vec2 normalize() const;
		float cross(const Vec2& other) const;
		bool operator==(const Vec2& other) const;
		bool operator!=(const Vec2& other) const;
		float angle(const Vec2& other) const;
		Vec2 project(const Vec2& other) const;
		Vec2 reflect(const Vec2& other) const;
		Vec2 reject(const Vec2& other) const;
		Vec2 lerp(const Vec2& other, float t) const;
		Vec2 slerp(const Vec2& other, float t) const;
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
		Vec3(const Vec3& other) : x(other.x), y(other.y), z(other.z) {}

		Vec3& operator=(const Vec3& other);
		Vec3 operator+(const Vec3& other) const;
		Vec3& operator+=(const Vec3& other);
		Vec3 operator-(const Vec3& other) const;
		Vec3 operator-() const;
		Vec3& operator-=(const Vec3& other);
		Vec3 operator/(float scalar) const;
		Vec3& operator/=(float scalar);
		Vec3 operator*(float scalar) const;
		Vec3 operator*(double scalar) const;
		Vec3& operator*=(float scalar);
		Vec3& operator*=(double scalar);
		float dot(const Vec3& other) const;
		float length() const;
		Vec3 normalize() const;
		Vec3 cross(const Vec3& other) const;
		bool operator==(const Vec3& other) const;
		bool operator!=(const Vec3& other) const;
		float angle(const Vec3& other) const;
		Vec3 project(const Vec3& other) const;
		Vec3 reflect(const Vec3& other) const;
		Vec3 reject(const Vec3& other) const;
		Vec3 lerp(const Vec3& other, float t) const;
		Vec3 slerp(const Vec3& other, float t) const;
		const float& operator[](int index) const;
		float& operator[](int index);
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
	alignas(16) float data[4][4];

	Mat4();
	Mat4(float value);
	Mat4(float matrix[4][4]);
	Mat4(const Mat4& other);

	Mat4& operator=(const Mat4& other);
	Mat4 operator*(const Mat4& other) const;
	Mat4 transpose() const;

	static Mat4 translate(const Mat4& mat, const Vec3& v);
	static Mat4 rotate(const Mat4& mat, float angle, const Vec3& axis);
	static Mat4 scale(const Mat4& mat, const Vec3& scaleFactors);
	static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
	static Mat4 perspective(float fov, float aspect, float near, float far);

	float* operator[](int index);
	const float* operator[](int index) const;
};

Vec3 operator*(const Mat4& mat, const Vec3& vec);

#endif
