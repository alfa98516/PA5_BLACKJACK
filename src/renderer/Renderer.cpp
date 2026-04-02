
#include "Renderer.hpp"
#include "Macros.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

void Renderer::Draw() const {
    m_vao.Bind();
    m_ibo.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, m_ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawSprite(const Sprite& sprite) const {
    if (!sprite.visable)
        return;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(sprite.position, 0.0f));
    model = glm::scale(model, glm::vec3(sprite.size, 1.0f));

    glm::mat4 mvp = m_proj * model;

    sprite.texture->Bind(0);
    m_defaultShader.Bind();
    m_defaultShader.SetUniformMat4f("u_MVP", mvp);

    Draw();
}

void Renderer::DrawGlyph(const Sprite& sprite) {
    if (!sprite.visable)
        return;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(sprite.position, 0.0f));
    model = glm::scale(model, glm::vec3(sprite.size, 1.0f));

    glm::mat4 mvp = m_proj * model;

    sprite.texture->Bind(0);

    m_glyphShader.Bind();
    m_glyphShader.SetUniformMat4f("u_MVP", mvp);
    m_glyphShader.SetUniform3f("u_texColor", 0.0f, 0.0f, 0.0f); // black text
    Draw();
}

void Renderer::Clear() const { GLCall(glClear(GL_COLOR_BUFFER_BIT)); }

std::unique_ptr<Renderer> Renderer::instance = nullptr;

Renderer::Renderer()
    : m_vao(), m_vbo(vertecies, 4 * 4 * sizeof(float)), m_ibo(indices, 6),
      m_defaultShader("res/shaders/basic.shader"), m_glyphShader("res/shaders/glyphs.shader"),
      m_proj(glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 1.0f)) {
    m_layout.Push<float>(2);
    m_layout.Push<float>(2);
    m_vao.AddBuffer(m_vbo, m_layout);
}

Renderer& Renderer::Instance() {
    if (!instance) {
        instance.reset(new Renderer());
    }
    return *instance;
}

void Renderer::Shutdown() { instance.reset(); }
