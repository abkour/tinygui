#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include "ClientState.hpp"
#include "float2.hpp"

#include <memory>
#include <stdexcept>

struct InterfaceController;

class Window {

public:

	Window();
	Window(const float2 windowDimensions);
	~Window();

	void Update();
	void UpdatePrev();

	void resize(const float2 newWindowDimensions);

	ClientStateManager GetPeripheralState() const;
	GLFWwindow* GetWindow() { return window; }

protected:

	GLFWwindow* window;
	float2 windowDimensions;

	std::unique_ptr<InterfaceController> interfaceController;
};