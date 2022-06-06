#pragma once

enum class AttributeType {
	TG_FLOAT, TG_DOUBLE, TG_UNSIGNED_INT, TG_INT
};

class IVertexBufferDesc {

public:

	IVertexBufferDesc() = default;
	virtual ~IVertexBufferDesc() = 0 {}

	virtual void Bind() = 0 {}
	virtual void Initialize(int AttributeIdx, int AttributeComponentCount, AttributeType AttributeType, int Stride, void* Offset) = 0 {}
};