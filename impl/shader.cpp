#pragma once
#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static bool isShaderCompilationValid(GLuint shaderID, GLenum shaderType, std::string& errorMessage);
static bool isProgramLinkageValid(GLuint programID, std::string& errorMessage);

namespace tinygui {

Shader::Shader(const char* vertexshaderPath, const char* fragmentshaderPath) {
	//
	// Read the vertex shader code into a c string
	std::ifstream vsFile(vertexshaderPath, std::ios::binary);
	if (vsFile.fail()) {
		throw std::runtime_error("File missing. Check the existence of \"gui_shader.glsl.vs\" in folder ..\impl");
	}
	
	std::ostringstream vsFileStream;
	vsFileStream << vsFile.rdbuf();
	// Close the file, we don't need it anymore
	vsFile.close();
	std::string vsFileString = std::move(vsFileStream.str());
	const char* vsFileCString = vsFileString.c_str();

	//
	// Read the fragment shader code into a c string
	std::ifstream fsFile(fragmentshaderPath, std::ios::binary);
	if (fsFile.fail()) {
		throw std::runtime_error("File missing. Check the existence of \"gui_shader.glsl.vs\" in folder ..\impl");
	}

	std::ostringstream fsFileStream;
	fsFileStream << fsFile.rdbuf();
	// Close the file, we don't need it anymore
	fsFile.close();
	std::string fsFileString = std::move(fsFileStream.str());
	const char* fsFileCString = fsFileString.c_str();

	GLuint vertexShaderID = 0;
	GLuint fragmentShaderID = 0;
	
	//
	// Compile shader
	glShaderSource(vertexShaderID, 1, &vsFileCString, NULL);
	glShaderSource(fragmentShaderID, 1, &fsFileCString, NULL);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	std::string compilationStatusMessage;
	if (!isShaderCompilationValid(vertexShaderID, GL_VERTEX_SHADER, compilationStatusMessage)) {
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		throw std::runtime_error(compilationStatusMessage);
	}
	if (!isShaderCompilationValid(fragmentShaderID, GL_FRAGMENT_SHADER, compilationStatusMessage)) {
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		throw std::runtime_error(compilationStatusMessage);
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	std::string linkageStatusMessage;
	if (!isProgramLinkageValid(programID, linkageStatusMessage)) {
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		throw std::runtime_error(linkageStatusMessage);
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

Shader::~Shader() {
	glDeleteProgram(programID);
}

void Shader::bind() {
	glUseProgram(programID);
}

}

static bool isShaderCompilationValid(GLuint shaderID, GLenum shaderType, std::string& errorMessage) {
	int success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		char errorLog[512];
		glGetShaderInfoLog(shaderID, 512, NULL, errorLog);
		switch (shaderType) {
		case GL_VERTEX_SHADER:
			errorMessage += "VERTEX_SHADER::";
			break;
		case GL_FRAGMENT_SHADER:
			errorMessage += "FRAGMENT_SHADER::";
			break;
		default:
			errorMessage += "INCORRECT_SHADER_SPECIFIED::";
			break;
		}
		std::cout << "errorMessage: " << errorLog << '\n';
		errorMessage = errorMessage + "FAILED_COMPILATION. ERROR MESSAGE: " + errorLog;
		return false;
	}
	return true;
}

static bool isProgramLinkageValid(GLuint programID, std::string& errorMessage) {
	int success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		char errorLog[512];
		glGetShaderInfoLog(programID, 512, NULL, errorLog);
		errorMessage += "Program linkage error. Error message: " + std::string(errorLog);
		return false;
	}
	return true;
}