#pragma once
#include <cstdint>
#include <sys/types.h>
#include <vector>
struct VertexBufferElement {
    uint32_t type;
    uint32_t count;
    uint8_t normalized;
    static unsigned int GetSizeOfType(uint32_t);
};

class VertexBufferLayout {
  private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

  public:
    VertexBufferLayout() : m_Stride(0) {}
    template <typename T> void Push(unsigned int count);
    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};
