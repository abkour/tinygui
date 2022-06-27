#pragma once
#include "point2.hpp"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <shaderdirect.hpp>

#include <algorithm>

namespace tinygui {

struct TGUIHistogram {

	TGUIHistogram(	const unsigned pPollingDistance,
					const unsigned pPollCount, 
					const float pLow, 
					const float pHigh);

	void plot(float point);

private:

	unsigned currentPoll;

	unsigned pollingDistance; // Distance between two polls
	unsigned pollCount;
	float screenPosX, screenPosY;
	float screenDimX, screenDimY;
	float low, high;

	// Rendering
	GLuint vao, vbo;
	ShaderWrapper histogramShader;

};

}