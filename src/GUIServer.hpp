#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include "float2.hpp"
#include "float3.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

#include "ClientState.hpp"
#include "Texture2D.hpp"

class GUIServer {

private:

	struct ServerImpl;
	std::unique_ptr<ServerImpl> sImpl;

public:

	GUIServer();
	GUIServer(GLFWwindow* window);
	~GUIServer();

	void CreateCheckbox(const float2 pos,
						const float2 dim,
						const float3 bg_color,
						const float3 fg_color,
						bool initialState,
						unsigned int attachId);

	unsigned int CreateRectangle(	const float2 pos, 
									const float2 dim, 
									const float3 bg_color,
									unsigned int attachId);

	unsigned int CreateWindowedRectangle(	const float2 pos,
											const float2 dimBody,
											const float2 dimHead,
											const float3 header_color,
											const float3 body_color,
											unsigned int attachId);

	unsigned int LoadTexture(const char* texturePath);

	void UpdateState(const float2 CursorPos, const ClientStateManager pClientStateManager);
	void Render();
};