#pragma once

class float3 {

public:

	float3();
	float3(float v) : x(v), y(v), z(v) {}
	float3(float pX, float pY, float pZ)
		: x(pX)
		, y(pY)
		, z(pZ)
	{}

	float x, y, z;
};