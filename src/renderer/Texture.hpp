#pragma once
#include "Texture.hpp"
#include <cstdint>
#include <string>
class Texture {
    uint32_t m_RendererID;
    std::string m_Filepath;
    uint8_t* m_LocalBuffer;
    int32_t m_Width, m_Height, m_BPS;

  public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(uint32_t slot) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
    inline int GetBPS() const { return m_BPS; }
};
