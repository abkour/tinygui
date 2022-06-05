#include "WindowedRectangle.hpp"

WindowedRectangle::WindowedRectangle(std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc, std::shared_ptr<IVertexBuffer> pVertexBuffer, const Vec2 originBody, const Vec2 dimensionBody, const Vec2 dimensionHead, const unsigned int id)
	: origin(originBody)
	, dimensionBody(dimensionBody)
	, dimensionHead(dimensionHead)
{
	this->id = id;
	vertices = std::make_unique<float[]>(vertexCount * 2);

	vertices[0] = originBody.x; vertices[1] = originBody.y;
	vertices[2] = originBody.x + dimensionBody.x; vertices[3] = originBody.y;
	vertices[4] = originBody.x + dimensionBody.x; vertices[5] = originBody.y + dimensionBody.y;

	vertices[6] = originBody.x; vertices[7] = originBody.y;
	vertices[8] = originBody.x + dimensionBody.x; vertices[9] = originBody.y + dimensionBody.y;
	vertices[10] = originBody.x; vertices[11] = originBody.y + dimensionBody.y;

	vertices[12] = originBody.x; vertices[13] = originBody.y + dimensionBody.y;
	vertices[14] = originBody.x + dimensionBody.x; vertices[15] = originBody.y + dimensionBody.y;
	vertices[16] = originBody.x + dimensionBody.x; vertices[17] = originBody.y + dimensionBody.y + dimensionHead.y;

	vertices[18] = originBody.x; vertices[19] = originBody.y + dimensionBody.y;
	vertices[20] = originBody.x + dimensionBody.x; vertices[21] = originBody.y + dimensionBody.y + dimensionHead.y;
	vertices[22] = originBody.x; vertices[23] = originBody.y + dimensionBody.y + dimensionHead.y;


	pVertexBuffer->Bind();
	pVertexBuffer->Initialize(vertexCount * 2 * sizeof(float), vertices.get(), VertexBufferUsage::STATIC_DRAW);
	pVertexBufferDesc->Bind();
	pVertexBufferDesc->Initialize(0, 2, AttributeType::TG_FLOAT, 0, 0);

	VertexBufferDesc = pVertexBufferDesc;
	VertexBuffer = pVertexBuffer;
}

ObjectStatus WindowedRectangle::Update(const Vec2 CursorPosition, const ClientState pClientState) {
	Vec2 OffsetOrigin = origin + TranslationVector;
	if (CursorPosition.x - OffsetOrigin.x < dimensionBody.x
		&& CursorPosition.y > OffsetOrigin.y + dimensionBody.y
		&& CursorPosition.y < OffsetOrigin.y + dimensionBody.y + dimensionHead.y)
	{
		if (pClientState.LMB_Down) {
			Translate(pClientState.MouseDelta);
		}
		if (pClientState.RMB_Down) {
			return ObjectStatus::TERMINATED;
		}
		return ObjectStatus::DRAG;
	}
	return ObjectStatus::DEFAULT;
}

void WindowedRectangle::Translate(const Vec2 TranslationDelta) {
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
	glDrawArrays(GL_TRIANGLES, 0, 12);
}
