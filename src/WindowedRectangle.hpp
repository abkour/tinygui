#pragma once 
#include "IObject.hpp"
#include <memory>

#include "VertexBufferDesc.hpp"
#include "VertexBuffer.hpp"
#include "type_io.hpp"

#include <glad/glad.h>

class WindowedRectangle : public IObject {

public:

	WindowedRectangle() = default;
	WindowedRectangle(std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc, std::shared_ptr<IVertexBuffer> pVertexBuffer, const Vec2 originBody, const Vec2 dimensionBody, const Vec2 dimensionHead, const unsigned int id);

	std::size_t GetVertexCount() const override { return vertexCount; }
	float* GetVertices() const override { return vertices.get(); }

	virtual bool Update(const Vec2 CursorPosition, const ClientState pClientState) override;
	virtual void Render(unsigned int shaderID) override;
	virtual void Translate(const Vec2 TranslationDelta) override;

protected:

	Vec2 origin;
	Vec2 dimensionBody;
	Vec2 dimensionHead;

	const std::size_t vertexCount = 12;
	std::unique_ptr<float[]> vertices;

protected:

	std::shared_ptr<IVertexBufferDesc> VertexBufferDesc;
	std::shared_ptr<IVertexBuffer> VertexBuffer;
};