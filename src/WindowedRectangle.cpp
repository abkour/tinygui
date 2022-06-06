#include "WindowedRectangle.hpp"

WindowedRectangle::WindowedRectangle(	std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc, 
										std::shared_ptr<IVertexBuffer> pVertexBuffer, 
										std::shared_ptr<ITexture2D> pTexture2D, 
										const Vec2 originBody, 
										const Vec2 dimensionBody, 
										const Vec2 dimensionHead, 
										const unsigned int id)
											: origin(originBody)
											, dimensionBody(dimensionBody)
											, dimensionHead(dimensionHead)
{
	this->id = id;
	
	vertices = std::make_unique<float[]>(vertexCount * 2);
	// Body
	vertices[0] = originBody.x; vertices[1] = originBody.y;
	vertices[2] = originBody.x + dimensionBody.x; vertices[3] = originBody.y;
	vertices[4] = originBody.x + dimensionBody.x; vertices[5] = originBody.y + dimensionBody.y;
	vertices[6] = originBody.x; vertices[7] = originBody.y;
	vertices[8] = originBody.x + dimensionBody.x; vertices[9] = originBody.y + dimensionBody.y;
	vertices[10] = originBody.x; vertices[11] = originBody.y + dimensionBody.y;

	// Header
	vertices[12] = originBody.x; vertices[13] = originBody.y + dimensionBody.y;
	vertices[14] = originBody.x + dimensionBody.x; vertices[15] = originBody.y + dimensionBody.y;
	vertices[16] = originBody.x + dimensionBody.x; vertices[17] = originBody.y + dimensionBody.y + dimensionHead.y;
	vertices[18] = originBody.x; vertices[19] = originBody.y + dimensionBody.y;
	vertices[20] = originBody.x + dimensionBody.x; vertices[21] = originBody.y + dimensionBody.y + dimensionHead.y;
	vertices[22] = originBody.x; vertices[23] = originBody.y + dimensionBody.y + dimensionHead.y;


	texCoords = std::make_unique<float[]>(vertexCount * 2);
	// Body
	texCoords[0] = 0.f; texCoords[1] = 0.f;
	texCoords[2] = 1.f; texCoords[3] = 0.f;
	texCoords[4] = 1.f; texCoords[5] = 1.f;
	texCoords[6] = 0.f; texCoords[7] = 0.f;
	texCoords[8] = 1.f; texCoords[9] = 1.f;
	texCoords[10] = 0.f; texCoords[11] = 1.f;

	// Header
	texCoords[12] = 0.f; texCoords[13] = 0.f;
	texCoords[14] = 1.f; texCoords[15] = 0.f;
	texCoords[16] = 1.f; texCoords[17] = 1.f;
	texCoords[18] = 0.f; texCoords[19] = 0.f;
	texCoords[20] = 1.f; texCoords[21] = 1.f;
	texCoords[21] = 0.f; texCoords[23] = 1.f;
	
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
	Texture->Bind();
	Texture->BindToUnit(0);
	glUseProgram(shaderID);
	glUniform2fv(glGetUniformLocation(shaderID, "translation"), 1, &TranslationVector.x);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
