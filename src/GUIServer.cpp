#include "GUIServer.hpp"

#include "IObject.hpp"
#include "RootObject.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <shaderdirect.hpp>

#include "Rectangle.hpp"
#include "WindowedRectangle.hpp"

#include "VertexBufferDesc_OpenGL.hpp"
#include "VertexBuffer_OpenGL.hpp"

struct GUIServer::ServerImpl {

	ServerImpl();

	GLFWwindow* glfwWindow;
	ShaderWrapper uberShader;
	IObject* root;

	IObject* FindObjectById(const unsigned int id, IObject* node);
	int GenerateId();

	void UpdateState(IObject* root, const Vec2 CursorPosition, const ClientState pClientState);
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

int GUIServer::ServerImpl::GenerateId() {
	reservedIds++;
	return reservedIds;
}

void GUIServer::ServerImpl::UpdateState(IObject* node, const Vec2 CursorPosition, const ClientState pClientState) {
	for (auto it = node->attachedObjects.begin(); it != node->attachedObjects.end();) {
		bool IsObjectUpdated = (*it)->Update(CursorPosition, pClientState);
		if (IsObjectUpdated) {
			if ((*it)->GetTerminateStatus()) {
				it = node->attachedObjects.erase(it);
			} else {
				++it;
			}
		} else {
			UpdateState(*it, CursorPosition, pClientState);
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

unsigned int GUIServer::CreateRectangle(const Vec2 pos, const Vec2 dim, unsigned int attachId) {
	auto node = sImpl->FindObjectById(attachId, sImpl->root);
	// If node is nullptr, the user must have provided a bad id. Do nothing in that case
	if (node != nullptr) {
		std::shared_ptr<VertexBufferDesc_OpenGL> VertexBufferDesc = std::make_shared<VertexBufferDesc_OpenGL>();
		std::shared_ptr<VertexBuffer_OpenGL> VertexBuffer = std::make_shared<VertexBuffer_OpenGL>();

		unsigned int NewId = sImpl->GenerateId();
		node->attachedObjects.emplace_back(new Rectangle(VertexBufferDesc, VertexBuffer, pos, dim, NewId));
		return NewId;
	}
	return 0;
}

unsigned int GUIServer::CreateWindowedRectangle(const Vec2 pos, const Vec2 dimBody, const Vec2 dimHead, unsigned int attachId) {
	auto node = sImpl->FindObjectById(attachId, sImpl->root);
	// If node is nullptr, the user must have provided a bad id. Do nothing in that case
	if (node != nullptr) {
		std::shared_ptr<VertexBufferDesc_OpenGL> VertexBufferDesc = std::make_shared<VertexBufferDesc_OpenGL>();
		std::shared_ptr<VertexBuffer_OpenGL> VertexBuffer = std::make_shared<VertexBuffer_OpenGL>();
		
		unsigned int NewId = sImpl->GenerateId();
		node->attachedObjects.emplace_back(new WindowedRectangle(VertexBufferDesc, VertexBuffer, pos, dimBody, dimHead, NewId));
		return NewId;
	}
	return 0;
}

void GUIServer::UpdateState(const Vec2 CursorPosition, const ClientState pClientState) {
	sImpl->UpdateState(sImpl->root, CursorPosition, pClientState);
}

void GUIServer::Render() {
	sImpl->Render(sImpl->root);
}


