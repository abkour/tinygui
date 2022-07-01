#include "checkbox.hpp"

Checkbox::Checkbox(	std::shared_ptr<IVertexBufferDesc> pVertexBufferDesc,
					std::shared_ptr<IVertexBuffer> pVertexBuffer,
					float2 pOrigin, 
					float2 pDimension,
					float3 pBackgroundColor,
					float3 pForegroundColor,
					bool* pState) 
						: origin(pOrigin)
						, dimension(pDimension)
						, bg_color(pBackgroundColor)
						, fg_color(pForegroundColor)
						, IsBoxChecked(pState)
{
	// Vertices [0:5] are for the box itself
	// Vertices [6:9] are for the two lines forming a cross
	// When rendering the latter 4 vertices are only drawn
	// if "IsBoxChecked" is true.
	std::size_t vertexCount = 10;

	this->id = std::numeric_limits<unsigned int>::max();

	pVertexBuffer->Bind();
	pVertexBuffer->AllocateSpace(sizeof(float) * vertexCount * 6, VertexBufferUsage::STATIC_DRAW);
	pVertexBufferDesc->Bind();
	pVertexBufferDesc->Initialize(0, 2, AttributeType::TG_FLOAT, sizeof(float) * 6, reinterpret_cast<void*>(0));
	pVertexBufferDesc->Initialize(1, 4, AttributeType::TG_FLOAT, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 2));

	void* buffer = pVertexBuffer->Map(BufferAccessRights::WRITE_ONLY);
	float* fbuffer = reinterpret_cast<float*>(buffer);


	// Body
	for (int i = 0; i < 10 - 4; ++i) {
		fbuffer[2 + i * 6] = bg_color.x;
		fbuffer[3 + i * 6] = bg_color.y;
		fbuffer[4 + i * 6] = bg_color.z;
		fbuffer[5 + i * 6] = 0.f;
	}

	// Head
	for (int i = vertexCount - 4; i < vertexCount; ++i) {
		fbuffer[2 + i * 6] = fg_color.x;
		fbuffer[3 + i * 6] = fg_color.y;
		fbuffer[4 + i * 6] = fg_color.z;
		fbuffer[5 + i * 6] = 0.f;
	}

	// Box
	fbuffer[0] = origin.x;					fbuffer[1] = origin.y;
	fbuffer[6] = origin.x + dimension.x;	fbuffer[7] = origin.y;
	fbuffer[12] = origin.x + dimension.y;	fbuffer[13] = origin.y + dimension.y;

	fbuffer[18] = origin.x;					fbuffer[19] = origin.y;
	fbuffer[24] = origin.x + dimension.x;	fbuffer[25] = origin.y + dimension.y;
	fbuffer[30] = origin.x;					fbuffer[31] = origin.y + dimension.y;

	float offx = dimension.x * 0.25f;
	float offy = dimension.y * 0.25f;
	// Cross
	fbuffer[36] = origin.x + offx;					fbuffer[37] = origin.y + dimension.y - offy;
	fbuffer[42] = origin.x + dimension.x - offx;	fbuffer[43] = origin.y + offy;
	fbuffer[48] = origin.x + offy;					fbuffer[49] = origin.y + offy;
	fbuffer[54] = origin.x + dimension.x - offx;	fbuffer[55] = origin.y + dimension.y - offy;

	pVertexBuffer->Unmap();

	VertexBufferDesc = pVertexBufferDesc;
	VertexBuffer = pVertexBuffer;
}

ObjectStatus Checkbox::Update(const float2 CursorPosition, const ClientStateManager pClientStateManager) {
	float2 OffsetOrigin = origin + TranslationVector;
	if (CursorPosition.x > OffsetOrigin.x && CursorPosition.x < OffsetOrigin.x + dimension.x
		&& CursorPosition.y > OffsetOrigin.y && CursorPosition.y < OffsetOrigin.y + dimension.y) 
	{
		if (pClientStateManager.LMBPressedThisFrame()) {
			*IsBoxChecked = !(*IsBoxChecked);
		}
	}
	return ObjectStatus::DEFAULT;
}

void Checkbox::Render(GLuint shaderID) {
	VertexBufferDesc->Bind();
	VertexBuffer->Bind();
	glUseProgram(shaderID);
	glUniform2fv(glGetUniformLocation(shaderID, "translation"), 1, &TranslationVector.x);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glLineWidth(3);
	if (*IsBoxChecked) {
		glDrawArrays(GL_LINES, 6, 4);
	}
	glLineWidth(1);
}

void Checkbox::Translate(const float2 TranslationDelta) {
	TranslationVector = TranslationVector + TranslationDelta;
	for (auto&& childObject : attachedObjects) {
		childObject->Translate(TranslationDelta);
	}
}