#include "GUIServer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <../ext/stb_image.h>

#include "IObject.hpp"
#include "RootObject.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <shaderdirect.hpp>

#include "checkbox.hpp"
#include "Rectangle.hpp"
#include "WindowedRectangle.hpp"

#include "Texture2D_Opengl.hpp"
#include "VertexBufferDesc_OpenGL.hpp"
#include "VertexBuffer_OpenGL.hpp"

struct GUIServer::ServerImpl {

	ServerImpl();

public:

	// Texture
	std::vector<std::shared_ptr<ITexture2D>> Textures;
	std::shared_ptr<ITexture2D>* FindTextureByID(const int ID);

public:

	GLFWwindow* glfwWindow;
	ShaderWrapper uberShader;
	IObject* root;

	IObject* FindObjectById(const unsigned int id, IObject* node);
	int GenerateId();

	void UpdateState(IObject* root, const float2 CursorPosition, const ClientStateManager pClientStateManager);
	void Render(IObject* root);

private:

	int reservedIds = 0;
};

GUIServer::ServerImpl::ServerImpl() {
	glfwWindow = nullptr;
	root = new RootObject;

	ShaderWrapper shader(
		false,
		shader_p(GL_VERTEX_SHADER, "C://users//flora//source//repos//tinygui//shaders//test.glsl.vs"),
		shader_p(GL_FRAGMENT_SHADER, "C://users//flora//source//repos//tinygui//shaders//test.glsl.fs")
	);

	uberShader = std::move(shader);
}

IObject* GUIServer::ServerImpl::FindObjectById(const unsigned int id, IObject* node) {
	if (id == 0) {
		return root;
	}
	for (auto&& attachedObject : node->attachedObjects) {
		if (attachedObject->id == id) {
			return attachedObject;
		} else {
			auto RecAttachedObject = FindObjectById(id, attachedObject);
			if (RecAttachedObject != nullptr) {
				return RecAttachedObject;
			}
		}
	}
	return nullptr;
}

std::shared_ptr<ITexture2D>* GUIServer::ServerImpl::FindTextureByID(const int ID) {
	for (auto&& texture : Textures) {
		if (texture->GetTextureID() == ID) {
			return &texture;
		}
	}
	return nullptr;
}


int GUIServer::ServerImpl::GenerateId() {
	reservedIds++;
	return reservedIds;
}

void GUIServer::ServerImpl::UpdateState(IObject* node, const float2 CursorPosition, const ClientStateManager pClientStateManager) {
	for (auto it = node->attachedObjects.begin(); it != node->attachedObjects.end();) {
		ObjectStatus objectStatus = (*it)->Update(CursorPosition, pClientStateManager);
		if (objectStatus != ObjectStatus::DEFAULT) {
			if (objectStatus == ObjectStatus::TERMINATED) {
				it = node->attachedObjects.erase(it);
			} else {
				++it;
			}
		} else {
			UpdateState(*it, CursorPosition, pClientStateManager);
			++it;
		}
	}
}

void GUIServer::ServerImpl::Render(IObject* root) {
	uberShader.bind();
	for (auto&& childObject : root->attachedObjects) {
		childObject->Render(uberShader.id());
		Render(childObject);
	}
}

GUIServer::GUIServer() {}

GUIServer::GUIServer(GLFWwindow* window) {
	sImpl = std::make_unique<ServerImpl>();
	if (window == nullptr) {
		throw std::runtime_error("Expected GLFWwindow to be initialized, but pointer to GLFWwindow was nullptr!");
	}
	sImpl->glfwWindow = window;
}

GUIServer::~GUIServer() {}

void GUIServer::CreateCheckbox(	const float2 pos,
								const float2 dim,
								const float3 bg_color,
								const float3 fg_color,
								bool initialState,
								unsigned int attachId) 
{
	auto node = sImpl->FindObjectById(attachId, sImpl->root);
	if (node != nullptr) {
		std::shared_ptr<VertexBufferDesc_OpenGL> VertexBufferDesc = std::make_shared<VertexBufferDesc_OpenGL>();
		std::shared_ptr<VertexBuffer_OpenGL> VertexBuffer = std::make_shared<VertexBuffer_OpenGL>();
	
		node->attachedObjects.emplace_back(new Checkbox(VertexBufferDesc, VertexBuffer, pos, dim, bg_color, fg_color, initialState));
	}
}

unsigned int GUIServer::CreateRectangle(const float2 pos, 
										const float2 dim,
										const float3 bg_color,
										unsigned int attachId) 
{
	auto node = sImpl->FindObjectById(attachId, sImpl->root);
	// If node is nullptr, the user must have provided a bad id. Do nothing in that case
	if (node != nullptr) {
		std::shared_ptr<VertexBufferDesc_OpenGL> VertexBufferDesc = std::make_shared<VertexBufferDesc_OpenGL>();
		std::shared_ptr<VertexBuffer_OpenGL> VertexBuffer = std::make_shared<VertexBuffer_OpenGL>();

		unsigned int NewId = sImpl->GenerateId();
		node->attachedObjects.emplace_back(new Rectangle(VertexBufferDesc, VertexBuffer, pos, dim, bg_color, NewId));
		return NewId;
	}
	return 0;
}

unsigned int GUIServer::CreateWindowedRectangle(const float2 pos, 
												const float2 dimBody, 
												const float2 dimHead,
												const float3 header_color,
												const float3 body_color,
												unsigned int attachId) 
{
	auto node = sImpl->FindObjectById(attachId, sImpl->root);
	// If node is nullptr, the user must have provided a bad id. Do nothing in that case
	if (node != nullptr) {
		std::shared_ptr<VertexBufferDesc_OpenGL> VertexBufferDesc = std::make_shared<VertexBufferDesc_OpenGL>();
		std::shared_ptr<VertexBuffer_OpenGL> VertexBuffer = std::make_shared<VertexBuffer_OpenGL>();

		unsigned int NewId = sImpl->GenerateId();
		node->attachedObjects.emplace_back(new WindowedRectangle(VertexBufferDesc, VertexBuffer, pos, dimBody, dimHead, header_color, body_color, NewId));
		return NewId;
	}
	return 0;
}

unsigned int GUIServer::LoadTexture(const char* texturePath) {
	int width, height, comp;
	unsigned char* data = stbi_load(texturePath, &width, &height, &comp, 0);
	if (data == nullptr) {
		return std::numeric_limits<unsigned int>::max();
	}

	TextureFormat textureFormat;
	switch (comp) {
	case 4:
		textureFormat = TextureFormat::RGBA;
		break;
	default:
		textureFormat = TextureFormat::RGB;
		break;
	}

	std::shared_ptr<Texture2D_OGL> textureID = std::make_shared<Texture2D_OGL>();
	textureID->Bind();
	textureID->AllocateSpace(float2(width, height), textureFormat);
	textureID->UpdateContents(data, float2(width, height));
	textureID->GenerateMipmaps();
	
	unsigned int textureIDNum = textureID->GetTextureID();

	sImpl->Textures.emplace_back(textureID);
	
	return textureIDNum;
}

void GUIServer::UpdateState(const float2 CursorPosition, const ClientStateManager pClientStateManager) {
	sImpl->UpdateState(sImpl->root, CursorPosition, pClientStateManager);
}

void GUIServer::Render() {
	sImpl->Render(sImpl->root);
}


