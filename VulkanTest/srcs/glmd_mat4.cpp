#include "glmd.hpp"

// Constructor - Initializes to zero matrix
Mat4::Mat4() {
	for (int col = 0; col < 4; ++col)
		for (int row = 0; row < 4; ++row)
			data[col][row] = 0.0f;
}
// Constructor from float
Mat4::Mat4(float value) {
	for (int col = 0; col < 4; ++col)
		for (int row = 0; row < 4; ++row)
			data[col][row] = (col == row) ? value : 0.0f;
}

// Constructor from 2D array
Mat4::Mat4(float matrix[4][4]) {
	for (int col = 0; col < 4; ++col)
		for (int row = 0; row < 4; ++row)
			data[col][row] = matrix[col][row];
}

// Copy constructor
Mat4::Mat4(const Mat4& other) {
	for (int col = 0; col < 4; ++col)
		for (int row = 0; row < 4; ++row)
			data[col][row] = other.data[col][row];
}

// Assignment operator
Mat4& Mat4::operator=(const Mat4& other) {
	if (this != &other) {
		for (int col = 0; col < 4; ++col)
			for (int row = 0; row < 4; ++row)
				data[col][row] = other.data[col][row];
	}
	return *this;
}

// Element access
float* Mat4::operator[](int index) {
	return data[index];
}

const float* Mat4::operator[](int index) const {
	return data[index];
}

// Matrix multiplication - Column-major order
Mat4 Mat4::operator*(const Mat4& other) const {
	Mat4 result(0.0f);
	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			result.data[col][row] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.data[col][row] += data[k][row] * other.data[col][k];
			}
		}
	}
	return result;
}

// Matrix transpose
Mat4 Mat4::transpose() const {
	Mat4 result;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			result.data[j][i] = data[i][j];
	return result;
}

// Matrix translation - Column-major order
Mat4 Mat4::translate(const Mat4& mat, const Vec3& v) {
	Mat4 result = mat;
	for (int i = 0; i < 3; ++i) {
		result.data[3][i] = mat.data[0][i] * v.x + mat.data[1][i] * v.y + mat.data[2][i] * v.z + mat.data[3][i];
	}
	return result;
}


// Matrix rotation
Mat4 Mat4::rotate(const Mat4& mat, float angle, const Vec3& axis) {
	Vec3 normalizedAxis = axis.normalize();
	float rad = angle; // in radians
	float cosA = cos(rad);
	float sinA = sin(rad);
	float oneMinusCosA = 1.0f - cosA;

	// Calculate components of the rotation matrix
	float xx = normalizedAxis.x * normalizedAxis.x;
	float yy = normalizedAxis.y * normalizedAxis.y;
	float zz = normalizedAxis.z * normalizedAxis.z;
	float xy = normalizedAxis.x * normalizedAxis.y;
	float xz = normalizedAxis.x * normalizedAxis.z;
	float yz = normalizedAxis.y * normalizedAxis.z;
	float xSinA = normalizedAxis.x * sinA;
	float ySinA = normalizedAxis.y * sinA;
	float zSinA = normalizedAxis.z * sinA;

	// Create the rotation matrix
	Mat4 rotationMatrix;
	rotationMatrix[0][0] = xx * oneMinusCosA + cosA;
	rotationMatrix[0][1] = xy * oneMinusCosA + zSinA;
	rotationMatrix[0][2] = xz * oneMinusCosA - ySinA;
	rotationMatrix[1][0] = xy * oneMinusCosA - zSinA;
	rotationMatrix[1][1] = yy * oneMinusCosA + cosA;
	rotationMatrix[1][2] = yz * oneMinusCosA + xSinA;
	rotationMatrix[2][0] = xz * oneMinusCosA + ySinA;
	rotationMatrix[2][1] = yz * oneMinusCosA - xSinA;
	rotationMatrix[2][2] = zz * oneMinusCosA + cosA;
	rotationMatrix[3][3] = 1.0f;

	// Apply the rotation to the input matrix
	return mat * rotationMatrix;
}

// Matrix Scaling
Mat4 Mat4::scale(const Mat4& mat, const Vec3& scaleFactors) {
	Mat4 scaleMatrix(1.0f);
	scaleMatrix[0][0] *= scaleFactors.x;
	scaleMatrix[1][1] *= scaleFactors.y;
	scaleMatrix[2][2] *= scaleFactors.z;

	return mat * scaleMatrix;
}

// Matrix lookAt
Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
	Vec3 f = (center - eye).normalize();	// Forward vector (z-axis)
	Vec3 r = f.cross(up).normalize();		// Right vector (x-axis)
	Vec3 u = r.cross(f);					// Up vector (y-axis)

	Mat4 view;
	view[0][0] = r.x;
	view[1][0] = r.y;
	view[2][0] = r.z;
	view[3][0] = -r.dot(eye);

	view[0][1] = u.x;
	view[1][1] = u.y;
	view[2][1] = u.z;
	view[3][1] = -u.dot(eye);

	view[0][2] = -f.x;
	view[1][2] = -f.y;
	view[2][2] = -f.z;
	view[3][2] = f.dot(eye);

	view[3][3] = 1.0f;
	view[1][3] = view[2][3] = view[0][3] = 0.0f;

	return view;
}

// Matrix perspective
Mat4 Mat4::perspective(float fov, float aspect, float near, float far) {
	Mat4 result(0.0f);
	const float tanHalfFov = tan(fov / 2.0f);

	result.data[0][0] = 1.0f / (aspect * tanHalfFov);
	result.data[1][1] = 1.0f / tanHalfFov;
	result.data[2][2] = far / (near - far);
	result.data[2][3] = -1.0f;
	result.data[3][2] = -((far * near) / (far - near));

	return result;
}
