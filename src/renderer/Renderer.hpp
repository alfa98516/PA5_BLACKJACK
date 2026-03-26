#pragma once
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "VertexArray.hpp"
#include "glm/ext/matrix_float4x4.hpp"

class Renderer {
  public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear() const;
    void DrawSprite(const Sprite& sprite, const glm::mat4& proj, const VertexArray&,
                    const IndexBuffer&, Shader& shader) const;
};
