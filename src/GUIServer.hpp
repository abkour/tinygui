#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include "Vec2.hpp"
#include "Vec3.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

#include "ClientState.hpp"

class GUIServer {

private:

	struct ServerImpl;
	std::unique_ptr<ServerImpl> sImpl;

public:

	GUIServer();
	GUIServer(GLFWwindow* window);
	~GUIServer();

	unsigned int CreateRectangle(const Vec2 pos, const Vec2 dim, unsigned int attachId);
	unsigned int CreateWindowedRectangle(const Vec2 pos, const Vec2 dimBody, const Vec2 dimHead, unsigned int attachId);

	void UpdateState(const Vec2 CursorPos, const ClientState pClientState);
	void Render();
};