#pragma once
#include "window.hpp"

#include "type_io.hpp"

struct InterfaceController {
    ClientStateManager clientStateManager;
    bool initialEntry = true;
};

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    InterfaceController* interfaceController = reinterpret_cast<InterfaceController*>(glfwGetWindowUserPointer(window));

    if (interfaceController->initialEntry) {
        interfaceController->clientStateManager.currentState.MousePosition.x = xpos;
        interfaceController->clientStateManager.currentState.MousePosition.y = ypos;
        interfaceController->initialEntry = false;
    }

    interfaceController->clientStateManager.currentState.MouseDelta.x = xpos - interfaceController->clientStateManager.currentState.MousePosition.x;
    interfaceController->clientStateManager.currentState.MouseDelta.y = interfaceController->clientStateManager.currentState.MousePosition.y - ypos;
    interfaceController->clientStateManager.currentState.MousePosition.x = xpos;
    interfaceController->clientStateManager.currentState.MousePosition.y = ypos;
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    InterfaceController* interfaceController = reinterpret_cast<InterfaceController*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        interfaceController->clientStateManager.currentState.LMB_Down = true;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        interfaceController->clientStateManager.currentState.LMB_Down = false;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        interfaceController->clientStateManager.currentState.RMB_Down = true;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        interfaceController->clientStateManager.currentState.RMB_Down = false;
    }
}

Window::Window() {
	window = nullptr;
}

Window::Window(const float2 windowDimensions) 
	: windowDimensions(windowDimensions)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(windowDimensions.x, windowDimensions.y, "TinyGUI", NULL, NULL);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD!");
    }

    interfaceController = std::make_unique<InterfaceController>();

    // Enable user pointer
    glfwSetWindowUserPointer(window, interfaceController.get());

    // Enable keyboard callback

    // Enable mouse callback
    //glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

Window::~Window() {
    glfwTerminate();
}

void Window::Update() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (interfaceController->initialEntry) {
        interfaceController->clientStateManager.currentState.MouseDelta.x = xpos;
        interfaceController->clientStateManager.currentState.MouseDelta.y = ypos;
        interfaceController->initialEntry = false;
    }

    interfaceController->clientStateManager.currentState.MouseDelta.x = xpos - interfaceController->clientStateManager.currentState.MousePosition.x;
    interfaceController->clientStateManager.currentState.MouseDelta.y = interfaceController->clientStateManager.currentState.MousePosition.y - ypos;
    interfaceController->clientStateManager.currentState.MousePosition.x = xpos;
    interfaceController->clientStateManager.currentState.MousePosition.y = ypos;
}

void Window::UpdatePrev() {
    interfaceController->clientStateManager.UpdatePreviousState();
}

void Window::resize(const float2 newWindowDimensions) {}

ClientStateManager Window::GetPeripheralState() const {
    return interfaceController->clientStateManager;
}