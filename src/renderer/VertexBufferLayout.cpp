#include "VertexBufferLayout.hpp"
#include "Macros.hpp"
unsigned int VertexBufferElement::GetSizeOfType(uint32_t type) {
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

/*
 * @brief this is a stupid fucntion, MSVC allows excplicit template specialization for functions
 * but gcc does not, so i need to code it like this
 *
 */
template <typename T> void VertexBufferLayout::Push(unsigned int count) {

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
    m_Stride += VertexBufferElement::GetSizeOfType(type) * count;
}
