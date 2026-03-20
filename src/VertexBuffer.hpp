#include <cstdint>
class VertexBuffer {
    uint32_t m_RendererID;

  public:
    VertexBuffer(const void* data, uint32_t size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};
