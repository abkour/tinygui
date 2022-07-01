#pragma once
#include "GUIServer.hpp"
#include "window.hpp"

#include "FontEngine.hpp"

#include <chrono>
#include <iostream>

void GetMonitorDPI(GLFWwindow* window) {

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    int width_mm, height_mm;
    glfwGetMonitorPhysicalSize(monitor, &width_mm, &height_mm);

    std::cout << width_mm << ", " << height_mm << '\n';
}

int main() {

    float2 WindowResolution(1920, 1080);

    Window windowHandle(WindowResolution);
    glfwSwapInterval(0);

    GUIServer guiServer(windowHandle.GetWindow());

    ShaderWrapper shader(
        false,
        shader_p(GL_VERTEX_SHADER, "C:\\Users\\flora\\source\\repos\\tinygui\\shaders\\fontshader.glsl.vs"),
        shader_p(GL_FRAGMENT_SHADER, "C:\\Users\\flora\\source\\repos\\tinygui\\shaders\\fontshader.glsl.fs")
    );
    shader.bind();

    const char* FontSource = "C:\\Users\\flora\\source\\repos\\tinygui\\rsc\\consola.ttf";
    FontEngine fe(WindowResolution);
    fe.SetProjection(WindowResolution);
    fe.InitializeFace(FontSource, 24);
    fe.MakeAtlas(256, 256);
    
    const char* emptystr = "";
    const char* str = "This is a very long string that needs to be chopped into pieces to fit evenly on the screen if that makes any sense hehe. This is a very long string that needs to be chopped into pieces to fit evenly on the screen if that makes any sense hehe. This is a very long string that needs to be chopped into pieces to fit evenly on the screen if that makes any sense hehe. This is a very long string that needs to be chopped into pieces to fit evenly on the screen if that makes any sense hehe. This is a very long string that needs to be chopped into pieces to fit evenly on the screen if that makes any sense hehe. This is a very long string that needs to be chopped into pieces to fit evenly on the screen if that makes any sense hehe. This is a very long string that needs to be chopped into pieces to fit evenly on the screen if that makes any sense hehe. This is a very long string that needs to be chopped into pieces to fit evenly on the screen if that makes any sense hehe. This is a very long string that needs to be chopped into pieces to fit evenly on the screen if that makes any sense hehe. This is a very long string that needs to be chopped into pieces to fit evenly on the screen if that makes any sense hehe.";

    unsigned int stringId = fe.MakeString(str, float2(20, 900), float2(20, 0));

    bool k_pressed = false;
    bool z_pressed = false;

    while (!glfwWindowShouldClose(windowHandle.GetWindow())) {
        glClearColor(0.f, 0.f, 0.70f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (glfwGetKey(windowHandle.GetWindow(), GLFW_KEY_K) == GLFW_PRESS) {
            k_pressed = true;
        }

        if (glfwGetKey(windowHandle.GetWindow(), GLFW_KEY_K) == GLFW_RELEASE && k_pressed) {
            k_pressed = false;
            fe.AddSubstring(" Hello ", float2(20, 900), float2(20, 0), stringId);
            std::cout << "Adding substr!\n";
        }

        if (glfwGetKey(windowHandle.GetWindow(), GLFW_KEY_Z) == GLFW_PRESS) {
            z_pressed = true;
        }

        if (glfwGetKey(windowHandle.GetWindow(), GLFW_KEY_Z) == GLFW_RELEASE && z_pressed) {
            z_pressed = false;
            fe.AddSubstring("a", float2(20, 900), float2(20, 0), stringId);
        }

        fe.Render(stringId, shader, float2(20.f, 600.f));
        glfwSwapBuffers(windowHandle.GetWindow());

        glfwPollEvents();
        if (glfwGetKey(windowHandle.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(windowHandle.GetWindow(), GLFW_TRUE);
        }
    }

    return 0;

    float3 dgray(0.15f);
    float3 lgray(0.7f);
    float3 yellow(1.f, 1.f, 0);

    bool check = false;

    auto rect1 = guiServer.CreateRectangle(float2(-0.9f), float2(0.1f), dgray, 0);
    auto rect2 = guiServer.CreateRectangle(float2(-0.7f), float2(0.1f), dgray, 0);
    auto windowRect1 = guiServer.CreateWindowedRectangle(float2(-0.25f), float2(0.5), float2(0.1), dgray, lgray, 0);
    auto windowRect2 = guiServer.CreateWindowedRectangle(float2(0.7f), float2(0.1f), float2(0.f, 0.1), lgray, dgray, windowRect1);
    guiServer.CreateCheckbox(float2(0.4f, -0.4f), float2(0.2f), dgray, lgray, &check, windowRect2);

    while (!glfwWindowShouldClose(windowHandle.GetWindow())) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ClientStateManager clientStateManager = windowHandle.GetPeripheralState();

        clientStateManager.currentState.MousePosition = clientStateManager.currentState.MousePosition / float2(500, 350);
        clientStateManager.currentState.MousePosition.x = clientStateManager.currentState.MousePosition.x - 1.f;
        clientStateManager.currentState.MousePosition.y = 1.f - clientStateManager.currentState.MousePosition.y;

        clientStateManager.currentState.MouseDelta = clientStateManager.currentState.MouseDelta / float2(500, 350);
        guiServer.UpdateState(clientStateManager.currentState.MousePosition, clientStateManager);
        guiServer.Render();

        windowHandle.UpdatePrev();
        glfwSwapBuffers(windowHandle.GetWindow());
        glfwPollEvents();
        windowHandle.Update();

        if (check) {
            std::cout << "Checked!\n";
        } else {
            std::cout << "Not checked!\n";
        }

        if (glfwGetKey(windowHandle.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(windowHandle.GetWindow(), GLFW_TRUE);
        }
    }
}
