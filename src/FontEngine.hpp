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

	struct GlyphString {

		GlyphString(int pPposx, int pPposy, int pLength, GLuint pVao, GLuint pVbo)
			: pposx(pPposx)
			, pposy(pPposy)
			, length(pLength)
			, vao(pVao)
			, vbo(pVbo)
		{}

		int pposx, pposy;
		int length;
		GLuint vao, vbo;
	};

public:

	FontEngine(const float2 pWindowResolution);

	void InitializeFace(const char* PathToFont, const unsigned PixelSize);
	
	unsigned int MakeString(const std::string& input, const float2 LeftOffset, const float2 RightOffset);
	
	void AddSubstring(const std::string& substr, const float2 lOffset, const float2 rOffset, const unsigned int idx);

	void Render(unsigned int idx, ShaderWrapper& shader, float2 Offset);
	
	void SetProjection(float2 Resolution);

	void MakeAtlas(const int resx, const int resy);

	~FontEngine();

protected:

	std::vector<GlyphString> strings;

protected:

	float2 windowResolution;
	glm::mat4 ortho_projection;

	GLuint glyph_atlas;

protected:

	FT_Library library;
	FT_Face face;

	struct Glyph;
	std::map<FT_UInt, Glyph> glyphs;

protected:

	int maxGlyphHeight;

protected:

	GlyphAtlas glyphAtlas;
};