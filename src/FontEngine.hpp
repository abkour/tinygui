#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

#include <array>
#include <map>
#include <string>

#include <glad/glad.h>
#include <shaderdirect.hpp>

#include <glm.hpp>

#include "float2.hpp"

struct GlyphAtlas {
	std::map<unsigned char, std::array<float, 12>> texcoords;
};

class FontEngine {

public:

	FontEngine();

	void InitializeFace(const char* PathToFont, const unsigned PixelSize);
	void MakeGlyphs(const std::string& input, const float2 LeftOffset, const float2 RightOffset);
	
	void Render(ShaderWrapper& shader, float2 Offset);
	
	void SetProjection(float2 Resolution);

	void MakeAtlas(const int resx, const int resy);

	~FontEngine();

protected:

	GLuint atlas_vao;
	GLuint atlas_vbo;
	int nGlyphsToDraw = 0;

	glm::mat4 ortho_projection;

	GLuint glyph_atlas;

protected:

	FT_Library library;
	FT_Face face;

	struct Glyph;
	std::map<FT_UInt, Glyph> glyphs;

protected:

	void MakeGlyphsFromAtlas(const std::string& input, const float2 LeftOffset, const float2 RightOffset);

protected:

	int maxGlyphHeight;

protected:

	GlyphAtlas glyphAtlas;
	bool AtlasPresent = false;

};