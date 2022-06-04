#pragma once

class Vec2 {

public:

	Vec2() = default;
	Vec2(float x, float y) : x(x), y(y) {}

	Vec2& operator=(const Vec2& other) {
		x = other.x;
		y = other.y;
		return *this;
	}

	Vec2& operator/(const Vec2& other) {
		Vec2 res;
		res.x = x / other.x;
		res.y = y / other.y;
		return res;
	}

	Vec2& operator-(const Vec2& other) {
		Vec2 res;
		res.x = x - other.x;
		res.y = y - other.y;
		return res;
	}

	Vec2& operator+(const Vec2& other) {
		Vec2 res;
		res.x = x + other.x;
		res.y = y + other.y;
		return res;
	}

	float x, y;
};