#pragma once
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
class VertexArray {

    uint32_t m_RendererID;

  public:
    VertexArray();
    ~VertexArray();
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};
