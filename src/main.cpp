#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <stdexcept>

#include "GUIServer.hpp"
#include "type_io.hpp"
#include "window.hpp"

int main() {
    Window windowHandle(Vec2(1000, 700));

    GUIServer guiServer(windowHandle.GetWindow());

    auto rect1 = guiServer.CreateRectangle(Vec2(-0.9f, -0.9f), Vec2(0.1f, 0.1f), 0);
    auto rect2 = guiServer.CreateRectangle(Vec2(-0.7f, -0.7f), Vec2(0.1f, 0.1f), 0);
    auto windowRect1 = guiServer.CreateWindowedRectangle(Vec2(-0.25f, -0.25f), Vec2(0.5, 0.5), Vec2(0.1, 0.1), 0);

    while (!glfwWindowShouldClose(windowHandle.GetWindow())) {
        glClearColor(0.f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ClientState clientState = windowHandle.GetPeripheralState();

        clientState.MousePosition = clientState.MousePosition / Vec2(500, 350);
        clientState.MousePosition.x = clientState.MousePosition.x - 1.f;
        clientState.MousePosition.y = 1.f - clientState.MousePosition.y;

        clientState.MouseDelta = clientState.MouseDelta / Vec2(500, 350);
        guiServer.UpdateState(clientState.MousePosition, clientState);
        guiServer.Render();

        glfwSwapBuffers(windowHandle.GetWindow());
        glfwPollEvents();
    }
}