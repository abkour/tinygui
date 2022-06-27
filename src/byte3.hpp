#pragma once
#include <cstdint>

struct byte3 {
		
	byte3() = default;
	byte3(uint8_t pX, uint8_t pY, uint8_t pZ)
		: x(pX)
		, y(pY)
		, z(pZ)
	{}

	uint8_t x, y, z;
};