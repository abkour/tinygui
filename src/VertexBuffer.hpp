#pragma once

enum class VertexBufferUsage {
	STATIC_DRAW = 0,
	DYNAMIC_DRAW
};

enum class BufferAccessRights {
	READ_ONLY,
	WRITE_ONLY,
	READ_WRITE
};

class IVertexBuffer {

public:

	IVertexBuffer() = default;
	virtual ~IVertexBuffer() = 0 {}

	virtual void AllocateSpace(const std::size_t size, VertexBufferUsage Usage) = 0 {}
	virtual void Bind() = 0 {}
	virtual void* Map(BufferAccessRights Access) = 0 {}
	virtual void Unmap() = 0 {}
	virtual void SubInitialize(int Offset, int Size, void* Source) = 0 {}

};