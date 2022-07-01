#pragma once 
#include "float3.hpp"
#include "IObject.hpp"
#include <memory>

#include "Texture2D.hpp"
#include "VertexBufferDesc.hpp"
#include "VertexBuffer.hpp"
#include "type_io.hpp"

#include <glad/glad.h>

class WindowedRectangle : public IObject {

public:

	WindowedRectangle() = default;
	WindowedRectangle(	std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc, 
						std::shared_ptr<IVertexBuffer> pVertexBuffer, 
						const float2 originBody, 
						const float2 dimensionBody, 
						const float2 dimensionHead,
						const float3 pHeaderColor,
						const float3 pBodyColor,
						const unsigned int id);

	float2 GetTranslationVector() const { return TranslationVector; }

	virtual ObjectStatus Update(const float2 CursorPosition, const ClientStateManager pClientStateManager) override;
	virtual void Render(unsigned int shaderID) override;
	virtual void Translate(const float2 TranslationDelta) override;

protected:

	float2 TranslationVector;

	float2 origin;
	float2 dimensionBody;
	float2 dimensionHead;

	const std::size_t vertexCount = 12;
	std::unique_ptr<float[]> vertices;

	float3 header_color;
	float3 body_color;

protected:

	std::shared_ptr<IVertexBufferDesc> VertexBufferDesc;
	std::shared_ptr<IVertexBuffer> VertexBuffer;
};