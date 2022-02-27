#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>

#include <glshader.hpp>

#include <iostream>
#include <map>
#include <string>

#include "point2.hpp"


namespace tinygui {

struct Character {

	Character(GLuint pTextureID, Point2 pQuadmin, Point2 pQuadmax, uint32_t pAdvance)  
		: textureID(pTextureID)
		, quadmin(pQuadmin)
		, quadmax(pQuadmax)
		, advance(pAdvance)
	{}

	GLuint textureID;
	Point2 quadmin, quadmax;
	uint32_t advance;
};

struct FontEngine {

	FontEngine(const char* pathToFont = "../rsc/consolas.fft");

	void renderText(const std::string& text);

private:

	std::map<char, Character> characters;
	GLuint quadvao, quadvbo;
};

}