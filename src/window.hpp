#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include "ClientState.hpp"
#include "Vec2.hpp"

#include <memory>
#include <stdexcept>

struct InterfaceController;

class Window {

public:

	Window();
	Window(const Vec2 windowDimensions);
	~Window();

	void resize(const Vec2 newWindowDimensions);

	ClientState GetPeripheralState() const;

	GLFWwindow* GetWindow() { return window; }

protected:

	void foo(GLFWwindow* , double, double) {}

	GLFWwindow* window;
	Vec2 windowDimensions;

	std::unique_ptr<InterfaceController> interfaceController;
};