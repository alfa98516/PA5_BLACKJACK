#include "Texture.hpp"
#include "Macros.hpp"
#include "Shader.hpp"
#include "vendor/stb_image/stb_image.h"
#include <filesystem>

Texture::Texture(const std::string& path)
    : m_RendererID(0), m_Filepath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPS(0) {
    stbi_set_flip_vertically_on_load(1);
    std::filesystem::path fullPath = getExecutableDir().parent_path() / m_Filepath;

    std::cout << fullPath << '\n';

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

Texture::~Texture() { GLCall(glDeleteTextures(1, &m_RendererID)); }

void Texture::Bind(uint32_t slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
