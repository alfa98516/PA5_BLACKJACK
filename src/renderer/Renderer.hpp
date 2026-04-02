#pragma once
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "renderer/VertexBufferLayout.hpp"

const float vertecies[]{
    -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 1.0f}; // we only making boxes, see if i care

const uint32_t indices[] = {0, 1, 2, 2, 3, 0};

class Renderer {
    VertexArray m_vao;
    VertexBuffer m_vbo;
    IndexBuffer m_ibo;
    VertexBufferLayout m_layout;
    glm::mat4 m_proj;
    static std::unique_ptr<Renderer> instance;
    Shader m_defaultShader;
    Renderer();

  public:
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    static Renderer& Instance();
    static void Shutdown();
    void Draw() const;
    void Clear() const;
    void DrawSprite(const Sprite& sprite) const;
};
