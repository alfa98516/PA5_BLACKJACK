// Static libraries
#include "Deck.hpp"
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE // tells GLFW not to include any OpenGL headers itself
#include <GLFW/glfw3.h>

// Vendors (also just static libraries)
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Base C++ and/or C
#include <cstdint>
#include <iostream>
#include <sys/types.h>

// Self-written code
#include "IndexBuffer.hpp"
#include "Macros.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
const bool DEBUG = true;
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

int main() {
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
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Window", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Turn on vsync
    // side note: if you're using an NVIDIA graphics card, this might not work.
    // it gets completely overwritten by some NVIDIA drivers
    // Don't buy NVIDIA cards !
    glfwSwapInterval(1);

    // debug
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    // contain in its own scope, everything gets cleaned up before we terminate the window
    // avoids a nasty segfault
    // were storing a bunch of OpenGL objects within objects, every single C++ object has
    // destructors which delete the OpenGL objects via a system call (be it a VertexArray or
    // IndexBuffer). If we dont contain everything here within its own scope, the destructors are
    // called at return, but glfwTerminate() also has the function of destroying those OpenGL
    // objects. So without the scope it would look like this:
    // Objects created -> OpenGL objects created -> glfwTerminate called -> OpenGL objects deleted
    // -> object tries to delete opengl objects -> segfault.
    // hence the scope

    {
        float cardW = 300.0f; // pixels
        float cardH = 420.0f; // maintains 0.714:1 ratio
        float x = 0.0f, y = 0.0f;

        float vectors[] = {
            -cardW / 2, -cardH / 2, 0.0f, 0.0f, // 0 bottom left
            cardW / 2,  -cardH / 2, 1.0f, 0.0f, // 1 bottom right
            cardW / 2,  cardH / 2,  1.0f, 1.0f, // 2 top right
            -cardW / 2, cardH / 2,  0.0f, 1.0f  // 3 top left
        };

        uint32_t indices[] = {0, 1, 2, 2, 3, 0};

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(vectors, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj =
            glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
        glm::vec4 result = proj * vp;

        Shader shader("res/shaders/basic.shader");
        shader.Bind();

        Deck deck = Deck(true);
        Deck::Card c = deck.Draw();
        std::cout << c.GetImagePath() << '\n';
        Texture texture(c.GetImagePath());
        texture.Bind(0);

        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        shader.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui::StyleColorsDark();

        ImGui_ImplOpenGL3_Init("#version 330");

        float gap = (WINDOW_WIDTH - 2 * cardW) / 3.0f;
        float centerA = gap + cardW / 2.0f;
        float centerB = gap + cardW + gap + cardW / 2.0f;

        float centerY = WINDOW_HEIGHT / 2.0f;

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.6f, 1.0f);
        glm::vec3 translationA(centerA, centerY, 0);

        glm::vec3 translationB(centerB, centerY, 0);

        while (!glfwWindowShouldClose(window)) {

            /* any rendering happens after this */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);

                glm::mat4 mvp = proj * view * model;

                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);

                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

            if (DEBUG) {
                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, (float)WINDOW_WIDTH);
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, (float)WINDOW_WIDTH);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwTerminate();
    return 0;
}
