#pragma once
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
class VertexArray {

  public:
    VertexArray();
    ~VertexArray();
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};
