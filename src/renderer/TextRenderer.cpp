#include "renderer/TextRenderer.hpp"
#include "renderer/Macros.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Texture.hpp"
#include <iostream>

TextRenderer::TextRenderer() {
    FT_Error error = FT_Init_FreeType(&lib);

    if (error) {
        std::cerr << "FreeType not initialized\n";
        exit(error);
    }

    std::filesystem::path fullPath = getExecutableDir() / "res/fonts/Blackjack.ttf";
    error = FT_New_Face(lib, fullPath.parent_path().string().c_str(), 0, &font);
    if (error == FT_Err_Unknown_File_Format) {
        std::cerr << "File format unknown\n";
        exit(error);
    } else if (error) {
        std::cerr << "Font not loaded, something went wrong\n";
        exit(error);
    }
}

/*
 * @brief The font im using is a custom font that i made, it does not have lowercase letters which
 * means dont use those. You can implement those youreself if you like.
 * @param text: The text were rendering.
 * @param pos: The x, y coordinates of the text.
 */
void TextRenderer::Render(const std::string& text, glm::vec2 pos, uint32_t em) {
    FT_Error error = FT_Set_Pixel_Sizes(font, 0, em);
    if (error) {
        std::cerr << "Setting em failed\n";
        exit(error);
    }
    for (char c : text) {
        uint32_t idx = FT_Get_Char_Index(font, static_cast<uint64_t>(c));
        error = FT_Load_Glyph(font, idx, FT_LOAD_DEFAULT);
        if (error) {
            std::cerr << "Error loading char: " << c << '\n';
            exit(error);
        }

        font->glyph->bitmap;
    }
}

TextRenderer& TextRenderer::Instance() {
    static TextRenderer instance;
    return instance;
}
