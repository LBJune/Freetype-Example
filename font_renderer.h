#pragma once

#include <map>
#include <string>
#include <glad/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "font_renderer_utils.h"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    ivec2 Size;    // Size of glyph
    ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

class FontRenderer {
public:
    FontRenderer(const std::string& ttfPath, unsigned int fontSize, FT_Render_Mode renderMode = FT_RENDER_MODE_NORMAL, int dpiX = 72, int dpiY = 72);
    ~FontRenderer();
    
    void setColor(float r, float g, float b);
    void renderText(const std::string& text, float x, float y, float scale = 1.0f);
    
private:
    GLuint generateGlyphTexture(char32_t text, ivec2* glyphSize, ivec2* glyphBearing, GLuint * advance);
    void loadCharacters(const std::u32string& utf32_text);
    
    GLuint VAO, VBO;
    GLuint shaderProgram;

    std::map<char32_t, Character> Characters;
    FT_Library ft;
    FT_Face face;

    FT_Render_Mode renderMode;
    FT_UInt dpiX, dpiY;
    vec3 textColor = vec3(1.0f, 1.0f, 1.0f);
};