#pragma once
#include <iostream>

#include "Vec2.hpp"
#include "Vec3.hpp"

inline std::ostream& operator<<(std::ostream& os, const Vec2& v) {
	return os << v.x << ", " << v.y;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
	return os << v.x << ", " << v.y << ", " << v.z;
}