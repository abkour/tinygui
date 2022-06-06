#pragma once
#include "GUIServer.hpp"
#include "window.hpp"

int main() {
    Window windowHandle(Vec2(1000, 700));

    GUIServer guiServer(windowHandle.GetWindow());

    const char* jpegSource = "C:\\Users\\flora\\rsc\\textures\\bricks.jpg";
    const char* pngSource = "C:\\Users\\flora\\rsc\\textures\\test.png";
    unsigned int jpegTexture = guiServer.LoadTexture(jpegSource);
    unsigned int pngTexture = guiServer.LoadTexture(pngSource);

    auto rect1 = guiServer.CreateRectangle(Vec2(-0.9f, -0.9f), Vec2(0.1f, 0.1f), 0, jpegTexture);
    auto rect2 = guiServer.CreateRectangle(Vec2(-0.7f, -0.7f), Vec2(0.1f, 0.1f), 0, jpegTexture);
    auto windowRect1 = guiServer.CreateWindowedRectangle(Vec2(-0.25f, -0.25f), Vec2(0.5, 0.5), Vec2(0.1, 0.1), 0, pngTexture);
    auto windowRect2 = guiServer.CreateWindowedRectangle(Vec2(0.7f, 0.7f), Vec2(0.1f, 0.1f), Vec2(0.f, 0.1), windowRect1, pngTexture);
    auto rect4 = guiServer.CreateRectangle(Vec2(0.4f, -0.4f), Vec2(0.5f, 0.5f), windowRect2, jpegTexture);

    while (!glfwWindowShouldClose(windowHandle.GetWindow())) {
        glClearColor(0.f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        windowHandle.Update();

        ClientState clientState = windowHandle.GetPeripheralState();

        clientState.MousePosition = clientState.MousePosition / Vec2(500, 350);
        clientState.MousePosition.x = clientState.MousePosition.x - 1.f;
        clientState.MousePosition.y = 1.f - clientState.MousePosition.y;

        clientState.MouseDelta = clientState.MouseDelta / Vec2(500, 350);
        guiServer.UpdateState(clientState.MousePosition, clientState);
        guiServer.Render();

        glfwSwapBuffers(windowHandle.GetWindow());
        glfwPollEvents();

        if (glfwGetKey(windowHandle.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(windowHandle.GetWindow(), GLFW_TRUE);
        }
    }
}