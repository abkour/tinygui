#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>

#include <shaderdirect.hpp>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "point2.hpp"
#include <mat4.hpp>
#include <shaderdirect.hpp>

namespace tinygui {

struct Character {

	Character(GLuint pTextureID, Point2 pSize, Point2 pBearing, uint32_t pAdvance)  
		: textureID(pTextureID)
		, size(pSize)
		, bearing(pBearing)
		, advance(pAdvance)
	{}

	GLuint textureID;
	Point2 size, bearing;
	uint32_t advance;
};

struct FontEngine {

	FontEngine(const char* pathToFont = "C://Users//flora//source//repos//tinygui//rsc//consola.ttf");

	~FontEngine();

	void renderLine(const std::string& text, const unsigned xOff, const unsigned yOff);

	void renderMultiLine(const std::vector<std::string>& text, const unsigned xOff, const unsigned yOff);

private:

	unsigned maxAdvanceHeight;

	ShaderWrapper font_shader;

	std::map<char, Character> characters;
	GLuint quadvao, quadvbo;
};

}