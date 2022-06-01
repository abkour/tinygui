#include "VertexBufferDesc.hpp"

#include <glad/glad.h>

class VertexBufferDesc_OpenGL : public IVertexBufferDesc {

public:

	VertexBufferDesc_OpenGL() {
		glGenVertexArrays(1, &vao);
	}

	virtual ~VertexBufferDesc_OpenGL() { glDeleteVertexArrays(1, &vao); }

	virtual void Bind() override {
		glBindVertexArray(vao);
	}

	virtual void Initialize(int AttributeIdx, int AttributeComponentCount, AttributeType AttributeType, int Stride, int* Offset) override {
		GLuint glAttributeType = 0;
		switch (AttributeType) {
		case AttributeType::TG_FLOAT:
			glAttributeType = GL_FLOAT;
			break;
		case AttributeType::TG_DOUBLE:
			glAttributeType = GL_DOUBLE;
			break;
		case AttributeType::TG_INT:
			glAttributeType = GL_INT;
			break;
		case AttributeType::TG_UNSIGNED_INT:
			glAttributeType = GL_UNSIGNED_INT;
			break;
		default:
			glAttributeType = GL_NONE;
			break;
		}
		glEnableVertexAttribArray(AttributeIdx);
		glVertexAttribPointer(AttributeIdx, AttributeComponentCount, glAttributeType, GL_FALSE, Stride, Offset);
	}

protected:

	GLuint vao;

};