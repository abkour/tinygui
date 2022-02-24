#pragma once
#include "window.hpp"
#include <stdexcept>

namespace tinygui {

Window::Window(uint32_t resx, uint32_t resy, const char* windowTitle) {
	if (window != nullptr) {
		throw std::runtime_error("This winodw has already been initialized!");
	}

	if (!glfwInit()) {
		throw std::runtime_error("Error. GLFW could not be initialized!");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);

	auto primaryMonitor = glfwGetPrimaryMonitor();
	if (!primaryMonitor) {
		throw std::runtime_error("Error. GLFW could not detect any monitor!");
	}

	auto vidMode = const_cast<GLFWvidmode*>(glfwGetVideoMode(primaryMonitor));
	if (!vidMode) {
		throw std::runtime_error("Error. GLFW coudln't retrieve video mode of current monitor!");
	}

	window = glfwCreateWindow(resx, resy, windowTitle, NULL, NULL);

	if (!window) {
		throw std::runtime_error("Error. GLFW could not create window!");
	}

	glfwMakeContextCurrent(window);
	if (glfwGetError(NULL)) {
		throw std::runtime_error("Error. GLFW could not make context current!");
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Error. GLAD could not be loaded!");
	}
}

struct Cursor {

	Cursor(float xpos, float ypos)
		: xpos(xpos)
		, ypos(ypos)
		, xoff(0.f)
		, yoff(0.f)
	{}

	float xpos;
	float ypos;
	float xoff;
	float yoff;

	bool initialEntry = true;
};

static Cursor uniqueCursor(0.f, 0.f);

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	if (uniqueCursor.initialEntry) {
		uniqueCursor.xpos = xpos;
		uniqueCursor.ypos = ypos;
		uniqueCursor.initialEntry = false;
	}

	uniqueCursor.xoff = xpos - uniqueCursor.xpos;
	uniqueCursor.yoff = uniqueCursor.ypos - ypos;
	uniqueCursor.xpos = xpos;
	uniqueCursor.ypos = ypos;
}

void Window::enableCursorCallback() const {
	glfwSetCursorPosCallback(window, cursorPositionCallback);
}

float Window::getXoffset() const {
	return uniqueCursor.xoff;
}

float Window::getYoffset() const {
	return uniqueCursor.yoff;
}

void Window::resetCursorOffset() const {
	uniqueCursor.xoff = 0.f;
	uniqueCursor.yoff = 0.f;
}


}