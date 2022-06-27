#include "Rectangle.hpp"

Rectangle::Rectangle(	std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc, 
						std::shared_ptr<IVertexBuffer> pVertexBuffer, 
						const float2 origin,
						const float2 dimension,
						float3 pBackgroundColor,
						const unsigned int id)
	: origin(origin)
	, dimension(dimension)
	, bg_color(pBackgroundColor)
{
	this->id = id;
	
	pVertexBuffer->Bind();
	pVertexBuffer->AllocateSpace(sizeof(float) * 6 * vertexCount, VertexBufferUsage::STATIC_DRAW);
	pVertexBufferDesc->Bind();
	pVertexBufferDesc->Initialize(0, 2, AttributeType::TG_FLOAT, 6 * sizeof(float), reinterpret_cast<void*>(0));
	pVertexBufferDesc->Initialize(1, 4, AttributeType::TG_FLOAT, 6 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	
	void* buffer = pVertexBuffer->Map(BufferAccessRights::WRITE_ONLY);
	float* fbuffer = reinterpret_cast<float*>(buffer);

	// Body
	for (int i = 0; i < vertexCount; ++i) {
		fbuffer[2 + i * 6] = bg_color.x;
		fbuffer[3 + i * 6] = bg_color.y;
		fbuffer[4 + i * 6] = bg_color.z;
		fbuffer[5 + i * 6] = 0.f;
	}
	
	fbuffer[0] = origin.x;					fbuffer[1] = origin.y;
	fbuffer[6] = origin.x + dimension.x;	fbuffer[7] = origin.y;
	fbuffer[12] = origin.x + dimension.y;	fbuffer[13] = origin.y + dimension.y;

	fbuffer[18] = origin.x;					fbuffer[19] = origin.y;
	fbuffer[24] = origin.x + dimension.x;	fbuffer[25] = origin.y + dimension.y;
	fbuffer[30] = origin.x;					fbuffer[31] = origin.y + dimension.y;

	pVertexBuffer->Unmap();

	VertexBufferDesc = pVertexBufferDesc;
	VertexBuffer = pVertexBuffer;
}

ObjectStatus Rectangle::Update(const float2 CursorPosition, const ClientStateManager pClientStateManager) {
	float2 OffsetOrigin = origin + TranslationVector;
	if (CursorPosition.x > OffsetOrigin.x && CursorPosition.x < OffsetOrigin.x + dimension.x 
		&& CursorPosition.y > OffsetOrigin.y && CursorPosition.y < OffsetOrigin.y + dimension.y) {
		return ObjectStatus::TERMINATED;
	}
	return ObjectStatus::DEFAULT;
}

void Rectangle::Translate(const float2 TranslationDelta) {
	TranslationVector = TranslationVector + TranslationDelta;
	for (auto&& attachedObject : attachedObjects) {
		attachedObject->Translate(TranslationDelta);
	}
}

void Rectangle::Render(unsigned int shaderID) {
	VertexBufferDesc->Bind();
	VertexBuffer->Bind();
	glUseProgram(shaderID);
	glUniform2fv(glGetUniformLocation(shaderID, "translation"), 1, &TranslationVector.x);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

