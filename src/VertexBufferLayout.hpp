#pragma once

#include <cstdint>
#include <glad/gl.h>
#include <vector>
struct VertexBufferElement {
    uint32_t type;
    uint32_t count;
    bool normalized;
};

class VertexBufferLayout {
    std::vector<VertexBufferElement> m_Elements;
    uint32_t m_Stride;

  public:
    VertexBufferLayout();
    template <typename T> void Push(uint32_t count) { static_assert(false); }

    template <> void Push<float>(uint32_t count) {
        m_Elements.push_back({GL_FLOAT, count, false});
        m_Stride += sizeof(GLfloat);
    }

    template <> void Push<uint32_t>(uint32_t count) {
        m_Elements.push_back({GL_UNSIGNED_INT, count, false});
        m_Stride += sizeof(GLuint);
    }
    template <> void Push<uint8_t>(uint32_t count) {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count, false});
        m_Stride += sizeof(GLubyte);
    }

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline uint32_t GetStride() const { return m_Stride; }
};
