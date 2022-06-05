#include "Rectangle.hpp"

Rectangle::Rectangle(std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc, std::shared_ptr<IVertexBuffer> pVertexBuffer, const Vec2 origin, const Vec2 dimension, const unsigned int id)
	: origin(origin)
	, dimension(dimension)
{
	this->id = id;
	vertices = std::make_unique<float[]>(vertexCount * 2);
	vertices[0] = origin.x; vertices[1] = origin.y;
	vertices[2] = origin.x + dimension.x; vertices[3] = origin.y;
	vertices[4] = origin.x + dimension.y; vertices[5] = origin.y + dimension.y;

	vertices[6] = origin.x; vertices[7] = origin.y;
	vertices[8] = origin.x + dimension.x; vertices[9] = origin.y + dimension.y;
	vertices[10] = origin.x; vertices[11] = origin.y + dimension.y;

	pVertexBuffer->Bind();
	pVertexBuffer->Initialize(vertexCount * 2 * sizeof(float), vertices.get(), VertexBufferUsage::STATIC_DRAW);
	pVertexBufferDesc->Bind();
	pVertexBufferDesc->Initialize(0, 2, AttributeType::TG_FLOAT, 0, 0);

	VertexBufferDesc = pVertexBufferDesc;
	VertexBuffer = pVertexBuffer;
}

ObjectStatus Rectangle::Update(const Vec2 CursorPosition, const ClientState pClientState) {
	Vec2 OffsetOrigin = origin + TranslationVector;
	if (CursorPosition.x > OffsetOrigin.x && CursorPosition.x < OffsetOrigin.x + dimension.x 
		&& CursorPosition.y > OffsetOrigin.y && CursorPosition.y < OffsetOrigin.y + dimension.y) {
		return ObjectStatus::TERMINATED;
	}
	return ObjectStatus::DEFAULT;
}

void Rectangle::Translate(const Vec2 TranslationDelta) {
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

