#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <cstdint>

namespace tinygui {

struct Window {

	Window(uint32_t resx, uint32_t resy, const char *windowTitle);


	GLFWwindow* window = nullptr;
};

}