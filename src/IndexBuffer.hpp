#pragma once
#include <cstdint>
#include <sys/types.h>
class IndexBuffer {
    uint32_t m_RendererID;
    uint32_t m_Count;

  public:
    IndexBuffer(const uint32_t* data, uint32_t count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;
    inline uint32_t GetCount() const { return m_Count; }
};
