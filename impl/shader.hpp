#pragma once
#include <glad/glad.h>
#include <string_view>

namespace tinygui {

struct Shader {

	Shader(const char* vertexshaderPath, const char* fragmentshaderPath);
	~Shader();

	void bind();

private:

	GLuint programID;

};

}