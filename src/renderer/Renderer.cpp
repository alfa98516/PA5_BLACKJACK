#include "Renderer.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include <glad/gl.h>

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawSprite(const Sprite& sprite, const glm::mat4& proj, const VertexArray& va,
                          const IndexBuffer& ib, Shader& shader) const {
    if (!sprite.visable)
        return;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(sprite.position, 0.0f));
    model = glm::scale(model, glm::vec3(sprite.size, 1.0f));

    glm::mat4 mvp = proj * model;

    sprite.texture->Bind(0);
    shader.Bind();
    shader.SetUniformMat4f("u_MVP", mvp);
    Draw(va, ib, shader);
}

void Renderer::Clear() const { GLCall(glClear(GL_COLOR_BUFFER_BIT)); }
