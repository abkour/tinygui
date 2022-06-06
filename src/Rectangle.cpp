#include "Rectangle.hpp"

Rectangle::Rectangle(	std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc, 
						std::shared_ptr<IVertexBuffer> pVertexBuffer, 
						std::shared_ptr<ITexture2D> pTexture2D, 
						const Vec2 origin,
						const Vec2 dimension, 
						const unsigned int id)
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

	texCoords = std::make_unique<float[]>(vertexCount * 2);
	texCoords[0] = 0.f; texCoords[1] = 0.f;
	texCoords[2] = 1.f; texCoords[3] = 0.f;
	texCoords[4] = 1.f; texCoords[5] = 1.f;
	
	texCoords[6] = 0.f; texCoords[7] = 0.f;
	texCoords[8] = 1.f; texCoords[9] = 1.f;
	texCoords[10] = 0.f; texCoords[11] = 1.f;

	pVertexBuffer->Bind();
	pVertexBuffer->AllocateSpace(sizeof(float) * vertexCount * 4, VertexBufferUsage::STATIC_DRAW);
	pVertexBuffer->SubInitialize(0, sizeof(float) * vertexCount * 2, vertices.get());
	pVertexBuffer->SubInitialize(sizeof(float) * vertexCount * 2, sizeof(float) * vertexCount * 2, texCoords.get());
	pVertexBufferDesc->Bind();
	pVertexBufferDesc->Initialize(0, 2, AttributeType::TG_FLOAT, 2 * sizeof(float), reinterpret_cast<void*>(0));
	pVertexBufferDesc->Initialize(1, 2, AttributeType::TG_FLOAT, 2 * sizeof(float), reinterpret_cast<void*>(sizeof(float) * vertexCount * 2));

	Texture = pTexture2D;
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
	Texture->Bind();
	Texture->BindToUnit(0);
	glUseProgram(shaderID);
	glUniform2fv(glGetUniformLocation(shaderID, "translation"), 1, &TranslationVector.x);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

