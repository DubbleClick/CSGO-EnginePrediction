#pragma once

struct matrix3x4_t {
	float flMatrix[3][4];
};

class Vector {
public:
	float x, y, z;
	Vector(void);
	Vector(float X, float Y, float Z);
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	float operator[](int i) const;
	float& operator[](int i);
	inline void Zero();
	Vector& Clamp();
	float LengthSqr() {
		return (x*x + y*y + z*z);
	}
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;
	__forceinline Vector&	operator+=(const Vector &v);
	__forceinline Vector&	operator-=(const Vector &v);
	__forceinline Vector&	operator*=(const Vector &v);
	__forceinline Vector&	operator*=(float s);
	__forceinline Vector&	operator/=(const Vector &v);
	__forceinline Vector&	operator/=(float s);
	__forceinline Vector&	operator+=(float fl);
	__forceinline Vector&	operator-=(float fl);
	Vector& operator=(const Vector &vOther);
	Vector	operator-(const Vector& v) const;
	Vector	operator+(const Vector& v) const;
	Vector	operator*(const Vector& v) const;
	Vector	operator/(const Vector& v) const;
	Vector	operator*(float fl) const;
	Vector	operator/(float fl) const;

	float Length2D(void) const;
	float Length(void) const;
	Vector Normalize(void);
	float Dot(const Vector& vOther) const;
};

using QAngle = Vector;

inline float Vector::Dot(const Vector& vOther) const {
	const Vector& a = *this;
	return(a.x*vOther.x + a.y*vOther.y + a.z*vOther.z);
}

inline float Vector::Length(void) const {
	float root = 0.0f;
	float sqsr = x*x + y*y + z*z;
	__asm sqrtss xmm0, sqsr
	__asm movss root, xmm0
	return root;
}

inline float Vector::Length2D(void) const {
	float root = 0.0f;
	float sqst = x*x + y*y;
	__asm sqrtss xmm0, sqst
	__asm movss root, xmm0
	return root;
}

inline Vector Vector::Normalize(void) {
	float length = this->Length();
	if (!length)
		return *this;
	this->x /= length;
	this->y /= length;
	this->z /= length;

	return *this;
}

inline QAngle& QAngle::Clamp() {

	if (this->x < -89.0f)
		this->x = -89.0f;

	if (this->x > 89.0f)
		this->x = 89.0f;

	while (this->y < -180.0f)
		this->y += 360.0f;

	while (this->y > 180.0f)
		this->y -= 360.0f;

	this->z = 0.0f;

	return *this;
}

inline void Vector::Init(float ix, float iy, float iz) {
	x = ix; y = iy; z = iz;
}

inline Vector::Vector(float X, float Y, float Z) {
	x = X; y = Y; z = Z;
}

inline Vector::Vector(void) {}

inline void Vector::Zero() {
	x = y = z = 0.0f;
}

inline void VectorClear(Vector& a) {
	a.x = a.y = a.z = 0.0f;
}

inline Vector& Vector::operator=(const Vector &vOther) {
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

inline float& Vector::operator[](int i) {
	return ((float*)this)[i];
}

inline float Vector::operator[](int i) const {
	return ((float*)this)[i];
}

inline bool Vector::operator==(const Vector& src) const {
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool Vector::operator!=(const Vector& src) const {
	return (src.x != x) || (src.y != y) || (src.z != z);
}

__forceinline  Vector& Vector::operator+=(const Vector& v) {
	x += v.x; y += v.y; z += v.z;
	return *this;
}

__forceinline  Vector& Vector::operator-=(const Vector& v) {
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

__forceinline  Vector& Vector::operator*=(float fl) {
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

__forceinline  Vector& Vector::operator*=(const Vector& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

__forceinline Vector&	Vector::operator+=(float fl) {
	x += fl;
	y += fl;
	z += fl;
	return *this;
}

__forceinline Vector&	Vector::operator-=(float fl) {
	x -= fl;
	y -= fl;
	z -= fl;
	return *this;
}

__forceinline  Vector& Vector::operator/=(float fl) {
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}

__forceinline  Vector& Vector::operator/=(const Vector& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline Vector Vector::operator+(const Vector& v) const {
	Vector res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}


inline Vector Vector::operator-(const Vector& v) const {
	Vector res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline Vector Vector::operator*(float fl) const {
	Vector res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline Vector Vector::operator*(const Vector& v) const {
	Vector res;
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}

inline Vector Vector::operator/(float fl) const {
	Vector res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline Vector Vector::operator/(const Vector& v) const {
	Vector res;
	res.x = x / v.x;
	res.y = y / v.y;
	res.z = z / v.z;
	return res;
}