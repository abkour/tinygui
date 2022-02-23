#pragma once
#include <glad/glad.h>
#include <string_view>

namespace tinygui {

struct Shader {

	Shader(std::string_view& vertexshaderPath, std::string_view& fragmentshaderPath);
	~Shader();

	void bind();

private:

	GLuint programID;

};

}