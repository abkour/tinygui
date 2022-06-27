#pragma once
#include "float3.hpp"
#include "IObject.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferDesc.hpp"

#include <glad/glad.h>
#include <memory>

class Checkbox : public IObject {

public:

	Checkbox(	std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc,
				std::shared_ptr<IVertexBuffer> pVertexBuffer,
				float2 pOrigin, 
				float2 pDimension,
				float3 pBackgroundColor,
				float3 pForegroundColor,
				bool pInitialState);

	float2 GetTranslationVector() const { return TranslationVector; }

	std::size_t GetVertexCount() const override { return vertexCount; }
	float* GetVertices() const override { return vertices.get(); }

	virtual ObjectStatus Update(const float2 CursorPosition, const ClientStateManager pClientStateManager) override;
	virtual void Render(GLuint shaderID) override;
	virtual void Translate(const float2 TranslationDelta) override;

protected:

	float2 TranslationVector;

	float2 origin;
	float2 dimension;
	bool IsBoxChecked;

	float3 bg_color;
	float3 fg_color;

	// Vertices [0:5] are for the box itself
	// Vertices [6:9] are for the two lines forming a cross
	// When rendering the latter 4 vertices are only drawn
	// if "IsBoxChecked" is true.
	const std::size_t vertexCount = 10;
	std::unique_ptr<float[]> vertices;

	std::shared_ptr<IVertexBufferDesc> VertexBufferDesc;
	std::shared_ptr<IVertexBuffer> VertexBuffer;
};