#pragma once

#include "Renderer.hpp"
#include <cstdint>
#include <glad/gl.h>
#include <sys/types.h>
#include <type_traits>
#include <vector>
struct VertexBufferElement {
    uint32_t type;
    uint32_t count;
    uint8_t normalized;

    static unsigned int GetSizeOfType(uint32_t type) {
        switch (type) {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
  private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

  public:
    VertexBufferLayout();

    /*
     * @brief this is a stupid fucntion, MSVC allows excplicit template specialization for functions
     * but gcc does not, so i need to code it like this
     *
     */
    template <typename T> void Push(unsigned int count) {

        uint8_t normalized = GL_FALSE;
        uint32_t type = GL_FLOAT;

        if (std::is_same<T, float>::value) {

        } else if (std::is_same<T, uint32_t>::value) {
            type = GL_UNSIGNED_INT;
        } else if (std::is_same<T, uint8_t>::value) {
            type = GL_UNSIGNED_BYTE;
            normalized = GL_TRUE;
        } else {
            ASSERT(false);
        }

        m_Elements.push_back({type, count, normalized});
        m_Stride += VertexBufferElement::GetSizeOfType(type);
    }

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};
