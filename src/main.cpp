#include <cstdint>
#include <glad/gl.h>
#include <sys/types.h>
#define GLFW_INCLUDE_NONE // tells GLFW not to include any OpenGL headers itself
#include <GLFW/glfw3.h>
#include <iostream>

#include "IndexBuffer.hpp"
#include "Macros.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

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
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Window", nullptr, nullptr);

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
    // side note: if you're using an NVIDIA graphics card, this might not work
    // it gets completely overwritten with some NVIDIA drivers
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
        float vectors[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
            0.5f,  -0.5f, 1.0f, 0.0f, // 1
            0.5f,  0.5f,  1.0f, 1.0f, // 2
            -0.5f, 0.5f,  0.0f, 1.0f  // 3

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

        Shader shader("res/shaders/basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/one-with-nothing.jpg");
        texture.Bind(0);

        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        shader.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float incr = 0.005f;
        while (!glfwWindowShouldClose(window)) {

            /* any rendering happens after this */
            renderer.Clear();

            shader.Bind();

            renderer.Draw(va, ib, shader);
            if (r > 1.0f)
                incr = -0.005f;
            else if (r < 0.0f)
                incr = 0.005f;
            r += incr;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
