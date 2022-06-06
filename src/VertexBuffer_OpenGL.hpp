#pragma once
#include "VertexBuffer.hpp"

#include <glad/glad.h>

class VertexBuffer_OpenGL : public IVertexBuffer {

public:

	VertexBuffer_OpenGL() {
		glGenBuffers(1, &vbo);
	}
	virtual ~VertexBuffer_OpenGL() { glDeleteBuffers(1, &vbo); }

	virtual void AllocateSpace(const std::size_t size, VertexBufferUsage Usage) override {
		GLuint glUsage = 0;
		switch (Usage) {
		case VertexBufferUsage::STATIC_DRAW:
			glUsage = GL_STATIC_DRAW;
			break;
		case VertexBufferUsage::DYNAMIC_DRAW:
			glUsage = GL_DYNAMIC_DRAW;
			break;
		default:
			glUsage = GL_NONE;
			break;
		}
		glBufferData(GL_ARRAY_BUFFER, size, NULL, glUsage);
	}

	virtual void Bind() override {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}
	
	virtual void SubInitialize(int Offset, int Size, void* Source) override {
		GLuint glUsage = 0;
		glBufferSubData(GL_ARRAY_BUFFER, Offset, Size, Source);
	}

protected:

	GLuint vbo;
};