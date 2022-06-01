#pragma once
#include "VertexBuffer.hpp"

#include <glad/glad.h>

class VertexBuffer_OpenGL : public IVertexBuffer {

public:

	VertexBuffer_OpenGL() {
		glGenBuffers(1, &vbo);
	}
	virtual ~VertexBuffer_OpenGL() { glDeleteBuffers(1, &vbo); }

	virtual void Bind() override {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}
	
	virtual void Initialize(int Size, void* Source, VertexBufferUsage Usage) override {
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
		glBufferData(GL_ARRAY_BUFFER, Size, Source, glUsage);
	}

protected:

	GLuint vbo;
};