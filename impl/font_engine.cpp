#include "font_engine.hpp"

namespace tinygui {

FontEngine::FontEngine(const char* pathToFont) {

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		throw std::runtime_error("Freetype library could not be initialized!");
	}

	FT_Face face;
	if (FT_New_Face(ft, pathToFont, 0, &face)) {
		throw std::runtime_error("Path doesn't specify valid font or internal error in FFT!");
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character(
            texture,
            Point2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Point2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        ); 
        characters.insert(std::pair<char, Character>(c, character));
    }

    // We no longer need to interface with the FT library.
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // OpenGL demands a 4byte alignment on texture elements. Our fonts have 1 byte alignment.
    // Therefore, we need to set the pixel alignment to 1byte.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

}