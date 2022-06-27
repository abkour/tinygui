#include "FontEngine.hpp"

#include <iostream> // For debugging messages
#include <utility>

#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

#include "float2.hpp"
#include "type_io.hpp"

struct FontEngine::Glyph {
	Glyph(const unsigned pGlyphIndex, int pAdvance, float2 pBearing, float2 pDimension, unsigned char* pBuffer)
		: glyph_index(pGlyphIndex)
		, advance(pAdvance)
		, bearing(pBearing)
		, dimension(pDimension)
	{
		if(pBuffer != nullptr) {
			bufferSize = dimension.x * dimension.y;
			buffer = std::make_unique<unsigned char[]>(bufferSize);
			std::memcpy(buffer.get(), pBuffer, sizeof(unsigned char) * bufferSize);
		}
		bufferSize = 0;
	}

	Glyph(const Glyph& other)
		: advance(other.advance)
		, bearing(other.bearing)
		, dimension(other.dimension)
		, bufferSize(other.bufferSize)
	{
		buffer = std::make_unique<unsigned char[]>(other.bufferSize);
		std::memcpy(buffer.get(), other.buffer.get(), bufferSize);
	}

	Glyph(Glyph&& other) 
		: advance(other.advance)
		, bearing(other.bearing)
		, dimension(other.dimension)
		, bufferSize(other.bufferSize)
	{
		buffer = std::make_unique<unsigned char[]>(bufferSize);
		buffer.swap(other.buffer);
	}

	short bufferSize;
	std::unique_ptr<unsigned char[]> buffer;
	float2 bearing;
	float2 dimension;
	int advance;
	unsigned glyph_index;
};


FontEngine::FontEngine() {
	// Enable Alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	FT_Error error = FT_Init_FreeType(&library);
	if (error) {
		std::cout << "Error after call to (FT_init_FreeType)" << '\n';
	}
}

FontEngine::~FontEngine() {
	glDeleteVertexArrays(1, &atlas_vao);
	glDeleteBuffers(1, &atlas_vbo);
	glDeleteTextures(1, &glyph_atlas);
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}


void FontEngine::InitializeFace(const char* PathToFont, const unsigned PixelSize) {
	FT_Error error = FT_New_Face(library, PathToFont, 0, &face);
	if (error) {
		std::cout << "Error after call to (FT_New_Face) with path: " << PathToFont << "\n";
	}

	error = FT_Set_Pixel_Sizes(face, 0, PixelSize);
	if (error) {
		std::cout << "Could not set pixel size!\n";
	}

	if (FT_HAS_KERNING(face)) {
		std::cout << "Font supports kerning!\n";
	} else {
		std::cout << "No kerning support!\n";
	}

}

void FontEngine::MakeAtlas(const int resx, const int resy) {
	std::unique_ptr<unsigned char[]> data = std::make_unique<unsigned char[]>(resx * resy);

	int budget_x = resx;
	int budget_y = resy;

	unsigned maxLineHeight = 0;

	AtlasPresent = true;
	for (int i = ' '; i < 'z'; ++i) {
		auto glyph_index = FT_Get_Char_Index(face, i);

		if (glyph_index != 0) {

			FT_Error error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
			if (error) {
				std::cout << "Error loading glyph!\n";
			}

			for (int j = 0; j < face->glyph->bitmap.rows; ++j) {
				std::memcpy(&data[((resy - budget_y) * resx) + resx - budget_x + resx * j],
							&face->glyph->bitmap.buffer[face->glyph->bitmap.width * j],
							face->glyph->bitmap.rows);
			}

			const float x00 = (resx - budget_x) / (float)resx;
			const float y00 = (resy - budget_y) / (float)resy;
			const float x01 = x00 + (face->glyph->bitmap.width / (float)resx);
			const float y01 = y00 + (face->glyph->bitmap.rows / (float)resy);
			std::array<float, 12> texcoords = 
			{
				x00, y01,
				x01, y01,
				x01, y00, 
				x00, y01,
				x01, y00,
				x00, y00
			};

			glyphAtlas.texcoords.emplace(i, texcoords);

			maxLineHeight = std::max(maxLineHeight, (unsigned)face->glyph->bitmap.rows);
			maxGlyphHeight = std::max(maxGlyphHeight, (int)face->glyph->bitmap.rows);

			auto retpair = glyphs.insert(std::make_pair(i, // Key
				// Value
				Glyph(glyph_index,
					face->glyph->advance.x / 64,
					float2(face->glyph->metrics.horiBearingX / 64, face->glyph->metrics.horiBearingY / 64),
					float2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					nullptr)));
		
			budget_x -= face->glyph->bitmap.width;
			if (budget_x < 0) {
				budget_y -= maxLineHeight;
				if (budget_y >= maxLineHeight) {
					maxLineHeight = 0;
					budget_x = resx;
				} else {
					break;
				}
			}
		}
	}

	glGenTextures(1, &glyph_atlas);
	glBindTexture(GL_TEXTURE_2D, glyph_atlas);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, resx, resy, 0, GL_RED, GL_UNSIGNED_BYTE, data.get());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void FontEngine::MakeGlyphs(const std::string& input, const float2 LeftOffset, const float2 RightOffset) {
	if(AtlasPresent) {
		MakeGlyphsFromAtlas(input, LeftOffset, RightOffset);
	}
}

void FontEngine::MakeGlyphsFromAtlas(const std::string& input, const float2 LeftOffset, const float2 RightOffset) {
	if (atlas_vbo != 0) {
		glDeleteBuffers(1, &atlas_vbo);
		nGlyphsToDraw = 0;
	}
	std::vector<float> quadstore(input.size() * 12);
	std::vector<float> uvstore(input.size() * 12);

	bool bFindWordLength = true;
	int wordlength2 = 0;
	int wordpos = LeftOffset.x;
	int ppos = 0;
	int pposy = 0;
	for (int i = 0; i < input.size(); ++i) {
		auto it = glyphs.find(input[i]);

		// Find the length in font space of the current character string in order to determine 
		// whether we should render the character string on a new line or not.
		if (bFindWordLength && input[i] != ' ') {
			for (int j = i; j < input.size(); ++j) {
				if (input[j] == ' ') {
					break;
				} else {
					auto it2 = glyphs.find(input[j]);
					wordlength2 += it2->second.advance;
				}
			}
			bFindWordLength = false;
		}
		if (input[i] == ' ') {
			bFindWordLength = true;
			wordlength2 = 0;
		}

		if (input[i] == ' ') {
			wordpos = ppos + it->second.advance;
		} else {
			if (wordpos + wordlength2 + LeftOffset.x > 1000 - RightOffset.x) {
				pposy -= maxGlyphHeight * 1.3;
				wordpos = 0;
				ppos = 0;
			}
		}
		
		quadstore[i * 12] = ppos + it->second.bearing.x;
		quadstore[i * 12 + 1] = pposy + it->second.bearing.y - it->second.dimension.y;
		quadstore[i * 12 + 2] = ppos + it->second.bearing.x + it->second.dimension.x;
		quadstore[i * 12 + 3] = pposy + it->second.bearing.y - it->second.dimension.y;
		quadstore[i * 12 + 4] = ppos + it->second.bearing.x + it->second.dimension.x;
		quadstore[i * 12 + 5] = pposy + it->second.bearing.y;
		quadstore[i * 12 + 6] = ppos + it->second.bearing.x;
		quadstore[i * 12 + 7] = pposy + it->second.bearing.y - it->second.dimension.y;
		quadstore[i * 12 + 8] = ppos + it->second.bearing.x + it->second.dimension.x;
		quadstore[i * 12 + 9] = pposy + it->second.bearing.y;
		quadstore[i * 12 + 10] = ppos + it->second.bearing.x;
		quadstore[i * 12 + 11] = pposy + it->second.bearing.y;

		auto uvdata = glyphAtlas.texcoords.find(input[i])->second;
		uvstore[i * 12] = uvdata[0];
		uvstore[i * 12 + 1] = uvdata[1];
		uvstore[i * 12 + 2] = uvdata[2];
		uvstore[i * 12 + 3] = uvdata[3];
		uvstore[i * 12 + 4] = uvdata[4];
		uvstore[i * 12 + 5] = uvdata[5];
		uvstore[i * 12 + 6] = uvdata[6];
		uvstore[i * 12 + 7] = uvdata[7];
		uvstore[i * 12 + 8] = uvdata[8];
		uvstore[i * 12 + 9] = uvdata[9];
		uvstore[i * 12 + 10] = uvdata[10];
		uvstore[i * 12 + 11] = uvdata[11];

		ppos += it->second.advance;

		nGlyphsToDraw++;
	}
	
	if (atlas_vao == 0) {
		glGenVertexArrays(1, &atlas_vao);
	}
	glBindVertexArray(atlas_vao);
	glGenBuffers(1, &atlas_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, atlas_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (quadstore.size() + uvstore.size()), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, quadstore.size() * sizeof(float), quadstore.data());
	glBufferSubData(GL_ARRAY_BUFFER, quadstore.size() * sizeof(float), uvstore.size() * sizeof(float), uvstore.data());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(quadstore.size() * sizeof(float)));
}

void FontEngine::Render(ShaderWrapper& shader, float2 Offset) {
	shader.bind();
	shader.upload2fv(&Offset.x, "offset");
	shader.upload44fm(glm::value_ptr(ortho_projection), "projection");
	glBindTexture(GL_TEXTURE_2D, glyph_atlas);
	glBindTextureUnit(0, glyph_atlas);
	glBindVertexArray(atlas_vao);
	glDrawArrays(GL_TRIANGLES, 0, nGlyphsToDraw * 6);
}

void FontEngine::SetProjection(float2 Resolution) {
	ortho_projection = glm::ortho(0.f, Resolution.x, 0.f, Resolution.y);
}
