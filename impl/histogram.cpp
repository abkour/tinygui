#pragma once
#include "histogram.hpp"
#include <iostream>

namespace tinygui {

static float round_f(const float val) {
	return std::floor(val + 0.5f);
}

static float mapRange(float input, float x0, float x1, float y0, float y1) {
	float slope = 1.0 * (y1 - y0) / (x1 - x0);
	return y0 + round_f(slope * (input - x0));
}

TGUIHistogram::TGUIHistogram(	const unsigned pPollingDistance,
								const unsigned pPollCount, 
								const float pLow, 
								const float pHigh) 
{
	pollingDistance = pPollingDistance;
	pollCount = pPollCount;
	low = pLow;
	high = pHigh;
	currentPoll = 0;

	screenPosX = 300; screenDimX = 700;
	screenPosY = 400; screenDimY = 300;

	ShaderWrapper shader(
		false,
		shader_p(GL_VERTEX_SHADER, "C://Users//flora//source//repos//tinygui//impl//histogram.glsl.vs"),
		shader_p(GL_FRAGMENT_SHADER, "C://Users//flora//source//repos//tinygui//impl//histogram.glsl.fs")
	);
	histogramShader = std::move(shader);
	histogramShader.bind();

	glUniform1uiv(glGetUniformLocation(histogramShader.id(), "plotInterval"), 1, &pollingDistance);

	glm::mat4 projection = glm::ortho(0.f, 1920.f, 0.f, 1080.f);
	glUniformMatrix4fv(glGetUniformLocation(histogramShader.id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	const float graphcolor[] = { 1.f, 0.66f, 0.f };
	glUniform3fv(glGetUniformLocation(histogramShader.id(), "graphColor"), 1, graphcolor);

	Point2* bufferData = new Point2[pollCount];
	for (int i = 0; i < pollCount; ++i) {
		bufferData[i].x = screenPosX + screenDimX + i * pollingDistance;
		bufferData[i].y = (high + low) / 2.f;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pollCount * sizeof(Point2), bufferData, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	delete[] bufferData;
}

void TGUIHistogram::plot(float point) {
	histogramShader.bind();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	const float clampedPoint = std::clamp(point, low, high);
	const float mappedPointY = mapRange(clampedPoint, low, high, screenPosY, screenPosY + screenDimY);

	//Point2 plottedPoint(pollingDistance * currentPoll + screenPosX, mappedPointY);
	Point2 plottedPoint((pollingDistance * currentPoll + screenPosX + screenDimX) / 1920.f, 0.5);
	std::cout << plottedPoint << '\n';
	glBufferSubData(GL_ARRAY_BUFFER, currentPoll, sizeof(Point2), &plottedPoint);

	glUniform1uiv(glGetUniformLocation(histogramShader.id(), "plotOffset"), 1, &currentPoll);
	glDrawArrays(GL_LINE_STRIP, 0, pollCount);

	currentPoll++;
	if (currentPoll % pollCount == 0) {
		currentPoll = 0;
	}
}

}
