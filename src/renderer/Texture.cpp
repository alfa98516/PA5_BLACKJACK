#include "Texture.hpp"
#include "Macros.hpp"
#include "Shader.hpp"
#include "stb_image/stb_image.h"
#include <cstring>
#include <filesystem>

Texture::Texture(const std::string& path)
    : m_RendererID(0), m_Filepath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPS(0) {
    stbi_set_flip_vertically_on_load(1);
    std::filesystem::path fullPath = getExecutableDir().parent_path() / m_Filepath;

    m_LocalBuffer = stbi_load(fullPath.string().c_str(), &m_Width, &m_Height, &m_BPS, 4);
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                        m_LocalBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_LocalBuffer) {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::Texture(uint8_t* glyph, uint32_t w, uint32_t h)
    : m_RendererID(0), m_Filepath(""), m_LocalBuffer(glyph), m_Width(w), m_Height(h), m_BPS(1) {

    size_t size = w * h;
    m_LocalBuffer = new uint8_t[size];
    memcpy(m_LocalBuffer, glyph, size);

    // Flip vertically
    uint32_t rowBytes = w;
    uint8_t* temp = new uint8_t[rowBytes];

    for (uint32_t y = 0; y < h / 2; ++y) {
        uint8_t* top = m_LocalBuffer + y * rowBytes;
        uint8_t* bottom = m_LocalBuffer + (h - 1 - y) * rowBytes;
        memcpy(temp, top, rowBytes);
        memcpy(top, bottom, rowBytes);
        memcpy(bottom, temp, rowBytes);
    }

    delete[] temp;
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // only one "color" channel

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE,
                        m_LocalBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &m_RendererID));
    if (m_LocalBuffer && m_Filepath.empty())
        delete[] m_LocalBuffer;
}

void Texture::Bind(uint32_t slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
