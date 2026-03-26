#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include <cstdint>

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count) : m_Count(count) {
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_RendererID); }
void IndexBuffer::Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); }
void IndexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
