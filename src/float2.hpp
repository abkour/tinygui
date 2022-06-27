#pragma once

class float2 {

public:

	float2() = default;
	float2(float v) : x(v), y(v) {}
	float2(float x, float y) : x(x), y(y) {}

	float2& operator=(const float2& other) {
		x = other.x;
		y = other.y;
		return *this;
	}

	float2& operator/(const float2& other) {
		float2 res;
		res.x = x / other.x;
		res.y = y / other.y;
		return res;
	}

	float2& operator-(const float2& other) {
		float2 res;
		res.x = x - other.x;
		res.y = y - other.y;
		return res;
	}

	float2& operator+(const float2& other) {
		float2 res;
		res.x = x + other.x;
		res.y = y + other.y;
		return res;
	}

	float x, y;
};