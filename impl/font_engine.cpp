#include "font_engine.hpp"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace tinygui {

TGUIFontEngine::TGUIFontEngine(const unsigned pixelsize, const char* pathToFont) {

    // OpenGL demands a 4byte alignment on texture elements. Our fonts have 1 byte alignment.
    // Therefore, we need to set the pixel alignment to 1byte.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		throw std::runtime_error("Freetype library could not be initialized!");
	}

	FT_Face face;
	if (FT_New_Face(ft, pathToFont, 0, &face)) {
		throw std::runtime_error("Path doesn't specify valid font or internal error in FFT!");
    }

	FT_Set_Pixel_Sizes(face, 0, pixelsize);

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
            face->glyph->advance.x >> 6
        ); 
     
        characters.insert(std::pair<char, Character>(c, character));
    }
    
    maxAdvanceHeight = face->max_advance_height >> 6;

    // We no longer need to interface with the FT library.
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    glGenVertexArrays(1, &quadvao);
    glGenBuffers(1, &quadvbo);
    glBindVertexArray(quadvao);
    glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));

    glm::mat4 projection = glm::ortho(0.f, 1920.f, 0.f, 1080.f);
    glm::mat4 transformFont(1.f);

    ShaderWrapper tmp_font_shader(
        false,
        shader_p(GL_VERTEX_SHADER, "C://Users//flora//source//repos//tinygui//impl//font_shader.glsl.vs"),
        shader_p(GL_FRAGMENT_SHADER, "C://Users//flora//source//repos//tinygui//impl//font_shader.glsl.fs")
    );
    font_shader = std::move(tmp_font_shader);
    font_shader.bind();
    glUniformMatrix4fv(glGetUniformLocation(font_shader.id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(font_shader.id(), "transformMatrix"), 1, GL_FALSE, glm::value_ptr(transformFont));

    float color[3] = { 1.f, 0.f, 0.f };
    glUniform3fv(glGetUniformLocation(font_shader.id(), "fontColor"), 1, color);

    // To only render the parts of the quad that are occupied by character pixels,
    // we enable alpha blending. See font_shader.glsl.fs/main for use case.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

TGUIFontEngine::~TGUIFontEngine() {
    for (auto& ch : characters) {
        glDeleteTextures(1, &ch.second.textureID);
    }
}

void TGUIFontEngine::setColor(const float* color) {
    font_shader.bind();
    glUniform3fv(glGetUniformLocation(font_shader.id(), "fontColor"), 1, color);
}

void TGUIFontEngine::setScale(const float scaleFactor) {
    glm::mat4 transformFont(scaleFactor);
    transformFont[3][3] = 1.f;
    font_shader.bind();
    glUniformMatrix4fv(glGetUniformLocation(font_shader.id(), "transformMatrix"), 1, GL_FALSE, glm::value_ptr(transformFont));
}


void TGUIFontEngine::renderLine(const std::string& text, const unsigned xOff, const unsigned yOff) {
    font_shader.bind();
    glBindVertexArray(quadvao);
    glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
    unsigned advance = 0;
    Point2 vertices[6];
    for (int i = 0; i < text.size(); ++i) {
        auto ch = characters.find(text[i]);
        Point2 bl(xOff + advance + ch->second.bearing.x, yOff - (ch->second.size.y - ch->second.bearing.y));
        Point2 tr(xOff + advance + ch->second.bearing.x + ch->second.size.x, yOff + ch->second.bearing.y);
        advance += ch->second.advance;
        
        // We have to flip the UV coordinates. That's why this looks kinda weird on first glance.
        Point2 vertices[12] = {
            { bl.x, tr.y },     Point2(0.f, 0.f),
            tr,                 Point2(1.f, 0.f),
            { tr.x, bl.y },     Point2(1.f, 1.f),
            
            { bl.x, tr.y },     Point2(0.f, 0.f),
            bl,                 Point2(0.f, 1.f),
            { tr.x, bl.y },     Point2(1.f, 1.f)
        };
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindTexture(GL_TEXTURE_2D, ch->second.textureID);
        glActiveTexture(GL_TEXTURE0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void TGUIFontEngine::renderMultiLine(const std::vector<std::string>& text, const unsigned xOff, const unsigned yOff) {
    font_shader.bind();
    glBindVertexArray(quadvao);
    glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
    unsigned advance = 0;
    unsigned vAdvance = 0;
    Point2 vertices[6];
    for (int i = 0; i < text.size(); ++i) {
        for (int j = 0; j < text[i].size(); ++j) {
            auto ch = characters.find(text[i][j]);
            Point2 bl(xOff + advance + ch->second.bearing.x, yOff - (ch->second.size.y - ch->second.bearing.y) - vAdvance);
            Point2 tr(xOff + advance + ch->second.bearing.x + ch->second.size.x, yOff + ch->second.bearing.y - vAdvance);
            advance += ch->second.advance;

            // We have to flip the UV coordinates. That's why this looks kinda weird on first glance.
            Point2 vertices[12] = {
                { bl.x, tr.y },     Point2(0.f, 0.f),
                tr,                 Point2(1.f, 0.f),
                { tr.x, bl.y },     Point2(1.f, 1.f),

                { bl.x, tr.y },     Point2(0.f, 0.f),
                bl,                 Point2(0.f, 1.f),
                { tr.x, bl.y },     Point2(1.f, 1.f)
            };

            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindTexture(GL_TEXTURE_2D, ch->second.textureID);
            glActiveTexture(GL_TEXTURE0);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        advance = 0;
        vAdvance += maxAdvanceHeight * 1.5;
    }
}


void TGUIFontEngine::renderMultiLineInBox(  const std::vector<std::string>& text,
                                        const unsigned xOff, const unsigned yOff,
                                        const unsigned maxWidth,
                                        const unsigned maxHeight) 
{
    font_shader.bind();
    glBindVertexArray(quadvao);
    glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
    unsigned advance = 0;
    unsigned vAdvance = 0;
    Point2 vertices[6];
    for (int i = 0; i < text.size(); ++i) {
        for (int j = 0; j < text[i].size(); ++j) {
            auto ch = characters.find(text[i][j]);

            const auto lx = xOff + advance + ch->second.bearing.x;
            const auto rx = xOff + advance + ch->second.bearing.x + ch->second.size.x;
            if (lx > maxWidth || rx > maxWidth) {
                advance = 0;
                vAdvance += maxAdvanceHeight * 0.4;
            }
 
            Point2 bl(xOff + advance + ch->second.bearing.x, yOff - (ch->second.size.y - ch->second.bearing.y) - vAdvance);
            Point2 tr(xOff + advance + ch->second.bearing.x + ch->second.size.x, yOff + ch->second.bearing.y - vAdvance);
            advance += ch->second.advance;

            // We have to flip the UV coordinates. That's why this looks kinda weird on first glance.
            Point2 vertices[12] = {
                { bl.x, tr.y },     Point2(0.f, 0.f),
                tr,                 Point2(1.f, 0.f),
                { tr.x, bl.y },     Point2(1.f, 1.f),

                { bl.x, tr.y },     Point2(0.f, 0.f),
                bl,                 Point2(0.f, 1.f),
                { tr.x, bl.y },     Point2(1.f, 1.f)
            };

            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindTexture(GL_TEXTURE_2D, ch->second.textureID);
            glActiveTexture(GL_TEXTURE0);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
}


void TGUIFontEngine::renderMultiLineInBox2( const std::vector<std::string>& text,
                                        const unsigned xOff, const unsigned yOff,
                                        const unsigned maxWidth,
                                        const unsigned maxHeight)
{
    font_shader.bind();
    float color[3] = { 0.f, 1.f, 1.f };
    glUniform3fv(glGetUniformLocation(font_shader.id(), "fontColor"), 1, color);

    std::cout.sync_with_stdio(false);

    glBindVertexArray(quadvao);
    glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
    unsigned advance = 0;
    unsigned vAdvance = 0;
    Point2 vertices[6];
    for (int i = 0; i < text.size(); ++i) {
        for (int j = 0; j < text[i].size(); ++j) {
            auto ch = characters.find(text[i][j]);

            const auto lx = xOff + advance + ch->second.bearing.x;
            const auto rx = xOff + advance + ch->second.bearing.x + ch->second.size.x;
            if (lx > maxWidth || rx > maxWidth) {
                advance = 0;
                vAdvance += maxAdvanceHeight * 0.4;
            }

            Point2 bl(xOff + advance + ch->second.bearing.x, yOff - (ch->second.size.y - ch->second.bearing.y) - vAdvance);
            Point2 tr(xOff + advance + ch->second.bearing.x + ch->second.size.x, yOff + ch->second.bearing.y - vAdvance);
            advance += ch->second.advance;

            // We have to flip the UV coordinates. That's why this looks kinda weird on first glance.
            Point2 vertices[12] = {
                { bl.x, tr.y },     Point2(0.f, 0.f),
                tr,                 Point2(1.f, 0.f),
                { tr.x, bl.y },     Point2(1.f, 1.f),

                { bl.x, tr.y },     Point2(0.f, 0.f),
                bl,                 Point2(0.f, 1.f),
                { tr.x, bl.y },     Point2(1.f, 1.f)
            };

            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindTexture(GL_TEXTURE_2D, ch->second.textureID);
            glActiveTexture(GL_TEXTURE0);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
}


}