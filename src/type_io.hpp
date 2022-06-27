#pragma once
#include <iostream>

#include "float2.hpp"
#include "float3.hpp"

#include <glm.hpp>

inline std::ostream& operator<<(std::ostream& os, const float2& v) {
	return os << v.x << ", " << v.y;
}

inline std::ostream& operator<<(std::ostream& os, const float3& v) {
	return os << v.x << ", " << v.y << ", " << v.z;
}

inline std::ostream& operator<<(std::ostream& os, const glm::mat4& m) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			os << m[i][j] << ' ';
		}
		os << '\n';
	}
	return os;
}