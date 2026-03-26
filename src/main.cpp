#include "core/ResourceManager.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "renderer/Renderer.hpp"
#include <glad/gl.h>
#include <type_traits>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "core/Input.hpp"
#include "core/ResourceManager.hpp"
#include "renderer/Sprite.hpp"
#include "renderer/Texture.hpp"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540
#define DEBUG true

int main(int argc, char* argv[]) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // set minimum version of GLFW and set the profile to core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw handles windows(not the os), they're freaky and different for every operating system.
    GLFWwindow* window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Blackjack", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to initialize GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
    }

    // Turn on vsync
    // side note: if you're using an NVIDIA graphics card, this might not work.
    // it gets completely overwritten by some NVIDIA drivers
    // Don't buy NVIDIA cards !
    glfwSwapInterval(1);

    // debug
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    float vertecies[]{-0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, 1.0f, 0.0f,
                      0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 1.0f};

    uint32_t indices[] = {0, 1, 2, 2, 3, 0};

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    VertexArray va;
    VertexBuffer vb(vertecies, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
    glm::vec4 result = proj * vp;

    Shader shader("res/shaders/basic.shader");
    shader.Bind();

    Renderer renderer;
    ResourceManager& resourceManager = ResourceManager::Instance();
    Input& input = Input::Instance();
    input.SetWindow(window);
    int clickCount = 0;
    input.RegisterCallbacks();
    std::shared_ptr<Texture> button_unpressed =
        resourceManager.Get("res/textures/ui/button_unpressed.png");
    std::shared_ptr<Texture> button_pressed =
        resourceManager.Get("res/textures/ui/buttonm_pressed.png");

    shader.SetUniform1i("u_Texture", 0);
    va.Unbind();
    vb.Unbind();
    ib.Unbind();

    shader.Unbind();

    Sprite button{glm::vec2{100, 200}, glm::vec2{100, 100}, nullptr, true};

    while (!glfwWindowShouldClose(window)) {
        input.Update();
        if (input.WasClicked()) {
            button_pressed->Bind(0);
            button.texture = button_pressed;
            clickCount++;
            std::cout << "Right button clicked: " << clickCount << " at position: " << "("
                      << input.GetPosition().x << ", " << input.GetPosition().y << ")\n";
            button_pressed->Unbind();
        }

        button_unpressed->Bind(0);
        button.texture = button_unpressed;
        renderer.DrawSprite(button, proj, va, ib, shader);
        shader.Bind();
        button_unpressed->Unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
