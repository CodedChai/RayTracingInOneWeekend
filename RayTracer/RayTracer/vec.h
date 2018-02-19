#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec {
public:
	vec() {}
	vec(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const vec& operator+() const { return *this; }
	inline vec operator-() const { return vec(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline vec& operator+=(const vec &v2);
	inline vec& operator-=(const vec &v2);
	inline vec& operator*=(const vec &v2);
	inline vec& operator/=(const vec &v2);
	inline vec& operator*=(const float t);
	inline vec& operator/=(const float t);

	inline float length() const {
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline float squaredLength() {
		return (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline void makeUnitVector();

	float e[3];
};

inline std::istream& operator>>(std::istream &is, vec &t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
}

inline std::ostream& operator<<(std::ostream &os, const vec &t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline void vec::makeUnitVector() {
	float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k; e[1] *= k; e[2] *= k;
}

inline vec operator+(const vec &v1, const vec &v2) {
	return vec(v1.e[0] + v2.e[0], v1.e[1] + v2[1], v1[2] + v2[2]);
}

inline vec operator-(const vec &v1, const vec &v2) {
	return vec(v1.e[0] - v2.e[0], v1.e[1] - v2[1], v1[2] - v2[2]);
}

inline vec operator*(const vec &v1, const vec &v2) {
	return vec(v1.e[0] * v2.e[0], v1.e[1] * v2[1], v1[2] * v2[2]);
}

inline vec operator/(const vec &v1, const vec &v2) {
	return vec(v1.e[0] / v2.e[0], v1.e[1] / v2[1], v1[2] / v2[2]);
}

inline vec operator*(float t, const vec &v) {
	return vec(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec operator*(const vec &v, float t) {
	return vec(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec operator/(vec v, float t) {
	return vec(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline float dot(const vec &v1, const vec &v2) {
	return (v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2]);
}

inline vec cross(const vec &v1, const vec &v2) {
	return vec((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
		-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]),
		(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline vec& vec::operator+=(const vec &v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline vec& vec::operator-=(const vec &v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline vec& vec::operator*=(const vec &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline vec& vec::operator/=(const vec &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline vec& vec::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec& vec::operator/=(const float t) {
	float k = 1.0 / t;
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline vec unitVector(vec v) {
	return v / v.length();
}