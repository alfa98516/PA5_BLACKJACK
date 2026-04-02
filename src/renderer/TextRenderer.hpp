#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <cstdint>
#include <string>

#include "glm/ext/vector_float2.hpp"

class Texture;

class TextRenderer {
    FT_Library lib;
    FT_Face font;
    TextRenderer();
    void GlyphToTexture(const FT_Bitmap&, Texture&);

  public:
    void Render(const std::string&, glm::vec2, uint32_t);
    TextRenderer& Instance();
};
