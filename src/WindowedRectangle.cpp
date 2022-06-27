#include "WindowedRectangle.hpp"

WindowedRectangle::WindowedRectangle(
					std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc,
					std::shared_ptr<IVertexBuffer> pVertexBuffer,
					const float2 originBody,
					const float2 dimensionBody,
					const float2 dimensionHead,
					const float3 pHeaderColor,
					const float3 pBodyColor,
					const unsigned int id)
						: origin(originBody)
						, dimensionBody(dimensionBody)
						, dimensionHead(dimensionHead)
						, header_color(pHeaderColor)
						, body_color(pBodyColor)
{
	this->id = id;

	pVertexBuffer->Bind();
	pVertexBuffer->AllocateSpace(sizeof(float) * 6 * vertexCount, VertexBufferUsage::STATIC_DRAW);
	pVertexBufferDesc->Bind();
	pVertexBufferDesc->Initialize(0, 2, AttributeType::TG_FLOAT, 6 * sizeof(float), reinterpret_cast<void*>(0));
	pVertexBufferDesc->Initialize(1, 4, AttributeType::TG_FLOAT, 6 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));

	float* fbuffer = reinterpret_cast<float*>(pVertexBuffer->Map(BufferAccessRights::WRITE_ONLY));

	// Body
	for (int i = 0; i < vertexCount / 2; ++i) {
		fbuffer[2 + i * 6] = body_color.x;
		fbuffer[3 + i * 6] = body_color.y;
		fbuffer[4 + i * 6] = body_color.z;
		fbuffer[5 + i * 6] = 0.f;
	}

	// header
	for (int i = 6; i < vertexCount; ++i) {
		fbuffer[2 + i * 6] = header_color.x;
		fbuffer[3 + i * 6] = header_color.y;
		fbuffer[4 + i * 6] = header_color.z;
		fbuffer[5 + i * 6] = 0.f;
	}

	// Body
	fbuffer[0] = originBody.x; fbuffer[1] = originBody.y;
	fbuffer[6] = originBody.x + dimensionBody.x; fbuffer[7] = originBody.y;
	fbuffer[12] = originBody.x + dimensionBody.x; fbuffer[13] = originBody.y + dimensionBody.y;
	fbuffer[18] = originBody.x; fbuffer[19] = originBody.y;
	fbuffer[24] = originBody.x + dimensionBody.x; fbuffer[25] = originBody.y + dimensionBody.y;
	fbuffer[30] = originBody.x; fbuffer[31] = originBody.y + dimensionBody.y;

	// Header
	fbuffer[36] = originBody.x;						fbuffer[37] = originBody.y + dimensionBody.y;
	fbuffer[42] = originBody.x + dimensionBody.x;	fbuffer[43] = originBody.y + dimensionBody.y;
	fbuffer[48] = originBody.x + dimensionBody.x;	fbuffer[49] = originBody.y + dimensionBody.y + dimensionHead.y;
	fbuffer[54] = originBody.x;						fbuffer[55] = originBody.y + dimensionBody.y;
	fbuffer[60] = originBody.x + dimensionBody.x;	fbuffer[61] = originBody.y + dimensionBody.y + dimensionHead.y;
	fbuffer[66] = originBody.x;						fbuffer[67] = originBody.y + dimensionBody.y + dimensionHead.y;
	
	pVertexBuffer->Unmap();

	VertexBufferDesc = pVertexBufferDesc;
	VertexBuffer = pVertexBuffer;
}

ObjectStatus WindowedRectangle::Update(const float2 CursorPosition, const ClientStateManager pClientStateManager) {
	float2 OffsetOrigin = origin + TranslationVector;
	
	if (CursorPosition.x > OffsetOrigin.x && CursorPosition.x < OffsetOrigin.x + dimensionBody.x
		&& CursorPosition.y > OffsetOrigin.y + dimensionBody.y
		&& CursorPosition.y < OffsetOrigin.y + dimensionBody.y + dimensionHead.y)
	{
		if (pClientStateManager.currentState.LMB_Down) {
			Translate(pClientStateManager.currentState.MouseDelta);
			return ObjectStatus::DRAG;
		}
		if (pClientStateManager.currentState.RMB_Down) {
			return ObjectStatus::TERMINATED;
		}
	}
	return ObjectStatus::DEFAULT;
}

void WindowedRectangle::Translate(const float2 TranslationDelta) {
	TranslationVector = TranslationVector + TranslationDelta;
	for (auto&& attachedObject : attachedObjects) {
		attachedObject->Translate(TranslationDelta);
	}
}

void WindowedRectangle::Render(unsigned int shaderID) {
	VertexBufferDesc->Bind();
	VertexBuffer->Bind();
	glUseProgram(shaderID);
	glUniform2fv(glGetUniformLocation(shaderID, "translation"), 1, &TranslationVector.x);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
