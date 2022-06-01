#pragma once
#include "Vec2.hpp"
#include "IObject.hpp"

#include "VertexBuffer.hpp"
#include "VertexBufferDesc.hpp"

#include <memory>

#include <glad/glad.h>

class Rectangle : public IObject {

public:

	Rectangle() = default;
	Rectangle(std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc, std::shared_ptr<IVertexBuffer> pVertexBuffer, const Vec2 origin, const Vec2 dimension, const unsigned int id);

	std::size_t GetVertexCount() const override { return vertexCount; }
	float* GetVertices() const override { return vertices.get(); }

	virtual bool Update(const Vec2 CursorPosition, const ClientState pClientState) override;
	
	virtual void Render(GLuint shaderID) override;

protected:

	Vec2 origin;
	Vec2 dimension;

	const std::size_t vertexCount = 6;
	std::unique_ptr<float[]> vertices;

protected:
	
	std::shared_ptr<IVertexBufferDesc> VertexBufferDesc;
	std::shared_ptr<IVertexBuffer> VertexBuffer;
};