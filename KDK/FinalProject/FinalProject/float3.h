#pragma once
#include <math.h>
class float3
{
public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float arr[3];
	};

	explicit float3() noexcept : x{ 0.f }, y{ 0.f }, z{ 0.f } {};
	float3(float X, float Y, float Z) noexcept : x{ X }, y{ Y }, z{ Z } {};

	float* operator&()
	{
		return arr;
	}

	const float3& operator=(const float3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return other;
	}

	float3 operator-(const float3& rhs) {
		return float3{ x - rhs.x, y - rhs.y , z - rhs.z };
	};

	float3 operator+(const float3& rhs) {
		return float3{ x + rhs.x, y + rhs.y , z + rhs.z };
	};

	float3 operator-=(const float3& rhs) {
		*this = *this - rhs;
		return *this;
	};

	float3 operator+=(const float3& rhs) {
		*this = *this + rhs;
		return *this;
	};

	float3 operator*(const float&a)const {
		return float3(x*a, y*a, z*a);
	};

	float3 operator/(const float&a)const {
		return float3(x/a, y/a, z/a);
	};

	float3 operator*=(const float& a) {
		*this = *this * a;
		return *this;
	};

	float3 operator/=(const float& a) {
		*this = *this / a;
		return *this;
	};

	const float& length() {
		return float(sqrt(x*x + y * y + z * z));
	}

	void normalize()
	{
		float le = length();

		if (le != 0)
			*this /= le;
	}

	const float distance(float3 q) const {
		return (q - *this).length();
	}

	void reset() {
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}

	const float3& cross(const float3& other)const {
		return float3(other.z*y - z * other.y, z*other.x - x * other.z, x*other.y - y * other.x);
	}

	const float& inner(const float3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
};