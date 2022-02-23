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


}