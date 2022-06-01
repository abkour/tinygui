#pragma once

enum class VertexBufferUsage {
	STATIC_DRAW = 0,
	DYNAMIC_DRAW
};

class IVertexBuffer {

public:

	IVertexBuffer() = default;
	virtual ~IVertexBuffer() = 0 {}

	virtual void Bind() = 0 {}
	virtual void Initialize(int Size, void* Source, VertexBufferUsage Usage) = 0 {}

};