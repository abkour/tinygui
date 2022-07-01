#pragma once
#include "float2.hpp"
#include "float3.hpp"
#include "IObject.hpp"

#include "Texture2D.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferDesc.hpp"

#include <memory>

#include <glad/glad.h>

class Rectangle : public IObject {

public:

	Rectangle() = default;
	Rectangle(	std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc, 
				std::shared_ptr<IVertexBuffer> pVertexBuffer,
				const float2 origin, 
				const float2 dimension, 
				const float3 pBackgroundColor,			
				const unsigned int id);

	float2 GetTranslationVector() const { return TranslationVector; }

	virtual ObjectStatus Update(const float2 CursorPosition, const ClientStateManager pClientStateManager) override;
	virtual void Render(GLuint shaderID) override;
	virtual void Translate(const float2 TranslationDelta) override;

protected:

	float2 TranslationVector;

	float2 origin;
	float2 dimension;

	const std::size_t vertexCount = 6;
	std::unique_ptr<float[]> vertices;

	float3 bg_color;

protected:
	
	std::shared_ptr<IVertexBufferDesc> VertexBufferDesc;
	std::shared_ptr<IVertexBuffer> VertexBuffer;
};