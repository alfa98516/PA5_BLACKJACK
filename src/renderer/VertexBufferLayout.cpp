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

    if constexpr (std::is_same_v<T, float>) {
        // nothing
    } else if constexpr (std::is_same_v<T, uint32_t>) {
        type = GL_UNSIGNED_INT;
    } else if constexpr (std::is_same_v<T, uint8_t>) {
        type = GL_UNSIGNED_BYTE;
        normalized = GL_TRUE;
    } else {
        static_assert(false, "Unsupported type for VertexBufferLayout::Push");
    }

    m_Elements.push_back({type, count, normalized});
    m_Stride += VertexBufferElement::GetSizeOfType(type) * count;
}
// Explicit instantiations for the types we use
template void VertexBufferLayout::Push<float>(unsigned int);
template void VertexBufferLayout::Push<uint32_t>(unsigned int);
template void VertexBufferLayout::Push<uint8_t>(unsigned int);
