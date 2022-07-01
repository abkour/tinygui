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
				bool* pState);

	float2 GetTranslationVector() const { return TranslationVector; }

	virtual float* GetVertices() const { return nullptr; }

	virtual ObjectStatus Update(const float2 CursorPosition, const ClientStateManager pClientStateManager) override;
	virtual void Render(GLuint shaderID) override;
	virtual void Translate(const float2 TranslationDelta) override;

	bool* IsBoxChecked;

protected:

	float2 TranslationVector;

	float2 origin;
	float2 dimension;

	float3 bg_color;
	float3 fg_color;

	std::shared_ptr<IVertexBufferDesc> VertexBufferDesc;
	std::shared_ptr<IVertexBuffer> VertexBuffer;
};