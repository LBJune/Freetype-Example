#include "font_renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <windows.h>
#include <vector>


GLuint CreateProgram(FT_Render_Mode renderMode) {
    switch (renderMode)
    {
    case FT_RENDER_MODE_LCD:
    case FT_RENDER_MODE_LCD_V:
        return createShaderProgram(getExeDirectory() + "\\resources\\shaders\\lcdFont.vert", getExeDirectory() + "\\resources\\shaders\\lcdFont.frag");
    case FT_RENDER_MODE_SDF:
        return createShaderProgram(getExeDirectory() + "\\resources\\shaders\\sdfFont.vert", getExeDirectory() + "\\resources\\shaders\\sdfFont.frag");
    default:
        return createShaderProgram(getExeDirectory() + "\\resources\\shaders\\normalFont.vert", getExeDirectory() + "\\resources\\shaders\\normalFont.frag");
        break;
    }
}

FontRenderer::FontRenderer(const std::string& ttfPath, unsigned int fontSize, FT_Render_Mode renderMode, int dpiX, int dpiY) :
dpiX(dpiX),
dpiY(dpiY),
renderMode(renderMode)
{
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face
    if (FT_New_Face(ft, ttfPath.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set the font size
    FT_Set_Char_Size(face, fontSize * 64, fontSize * 64, dpiX, dpiY);
    
    // Complie shader
    shaderProgram = CreateProgram(renderMode);
    
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

FontRenderer::~FontRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

GLuint FontRenderer::generateGlyphTexture(char32_t text, ivec2* glyphSize, ivec2* glyphBearing, GLuint* advance) {

    if (renderMode >= FT_RENDER_MODE_MAX) return 0;
    // Load character glyph 
    FT_UInt glyph_index = FT_Get_Char_Index(face, text);
    if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)) return 0;
    if (FT_Render_Glyph(face->glyph, renderMode)) return 0;

    int glyphWidth = face->glyph->bitmap.width;
    int glyphHeight = face->glyph->bitmap.rows;
    unsigned char* data = face->glyph->bitmap.buffer;
    GLenum dataFormat = GL_RED;
    std::vector<unsigned char> buffer;

    if (renderMode == FT_RENDER_MODE_MONO) {
        buffer.resize(glyphWidth * glyphHeight);
        unsigned char* ptr = buffer.data();
        int pitch = face->glyph->bitmap.pitch;

        for (int y = 0; y < glyphHeight; y++) {
            unsigned char* row = data + y * (pitch > 0 ? pitch : -pitch);
            if (pitch < 0) row = data + (glyphHeight - 1 - y) * (-pitch);

            for (int x = 0; x < glyphWidth; x++) {
                int byte_idx = x / 8;
                int bit_idx = 7 - (x % 8);

                int pixel = (row[byte_idx] >> bit_idx) & 0x01;
                *ptr++ = pixel ? 255 : 0;
            }
        }
        data = buffer.data();
    }
    else if (renderMode == FT_RENDER_MODE_LCD) {
        glyphWidth = face->glyph->bitmap.width / 3;
        dataFormat = GL_RGB;

        buffer.resize(glyphWidth * glyphHeight * 3);
        unsigned char* ptr = buffer.data();
        int pitch = face->glyph->bitmap.pitch;
        int is_bgr = 0;         // Subpixel order: 0=RGB, 1=BGR

        data = pitch > 0 ? data : data + (glyphHeight - 1) * (-pitch); // Start at first row or last row 

        for (int y = 0; y < glyphHeight; y++) {
            unsigned char* row = data + y * pitch;
            for (int x = 0; x < glyphWidth; x++) {
                ptr[0] = is_bgr ? row[2] : row[0];
                ptr[1] = row[1];
                ptr[2] = is_bgr ? row[0] : row[2];

                ptr += 3;
                row += 3;
            }
        }
        data = buffer.data();
    }
    else if (renderMode == FT_RENDER_MODE_LCD_V) {
        glyphHeight = face->glyph->bitmap.rows / 3;
        dataFormat = GL_RGB;

        buffer.resize(glyphWidth * glyphHeight * 3);
        unsigned char* ptr = buffer.data();
        int pitch = face->glyph->bitmap.pitch;
        int abs_pitch = pitch > 0 ? pitch : -pitch;
        int is_bgr = 0;         // Subpixel order: 0=RGB, 1=BGR

        data = pitch > 0 ? data : data + 3 * (glyphHeight - 1) * (-pitch); // Start at first row or last row 

        for (int y = 0; y < glyphHeight; y++) {

            unsigned char* row = data + 3 * y * pitch;
            unsigned char* row1 = row + abs_pitch;
            unsigned char* row2 = row1 + abs_pitch;

            for (int x = 0; x < glyphWidth; x++) {
                ptr[0] = is_bgr ? *row2++ : *row++;
                ptr[1] = *row1++;
                ptr[2] = is_bgr ? *row++ : *row2++;

                ptr += 3;
            }
        }
        data = buffer.data();
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, glyphWidth, glyphHeight, 0, dataFormat, GL_UNSIGNED_BYTE, data);

    //  Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glyphSize->x = glyphWidth;
    glyphSize->y = glyphHeight;

    glyphBearing->x = face->glyph->bitmap_left;
    glyphBearing->y = face->glyph->bitmap_top;

    *advance = face->glyph->advance.x;

    return texture;
}
void FontRenderer::loadCharacters(const std::u32string& utf32_text) {
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for (char32_t c : utf32_text)
    {
        if (Characters.find(c) != Characters.end()) continue;

        ivec2 glyphSize, glyphBearing;
        GLuint advance;
        GLuint texture = generateGlyphTexture(c, &glyphSize, &glyphBearing, &advance);
       
        if (texture == 0) continue;
        
        // Now store character for later use
        Character character = {
            texture,
            glyphSize,
            glyphBearing,
            advance
        };
        Characters.insert(std::pair<char32_t, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FontRenderer::setColor(float r, float g, float b) {
    textColor.x = r;
    textColor.y = g;
    textColor.z = b;
}

void FontRenderer::renderText(const std::string& text, float x, float y, float scale) {

    // support Chinese
    std::u32string& utf32_text = utf8_to_utf32(text);

    loadCharacters(utf32_text);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Activate corresponding render state	
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), textColor.x, textColor.y, textColor.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    //std::string::const_iterator c;
    for (char32_t c : utf32_text)
    {
        Character ch = Characters[c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { 2 * xpos / gWidth - 1,     2 * (ypos + h) / gHeight - 1,   0.0, 0.0 },
            { 2 * xpos / gWidth - 1,     2 * ypos / gHeight - 1,       0.0, 1.0 },
            { 2 * (xpos + w) / gWidth - 1, 2 * ypos / gHeight - 1,       1.0, 1.0 },

            { 2 * xpos / gWidth - 1,     2 * (ypos + h) / gHeight - 1,   0.0, 0.0 },
            { 2 * (xpos + w) / gWidth - 1, 2 * ypos / gHeight - 1,       1.0, 1.0 },
            { 2 * (xpos + w) / gWidth - 1, 2 * (ypos + h) / gHeight - 1,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}