#include <cstdint>
#include <glad/gl.h>
#include <string>
#include <sys/types.h>
#define GLFW_INCLUDE_NONE // tells GLFW not to include any OpenGL headers itself
#include <GLFW/glfw3.h>
#include <iostream>

#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "VertexBuffer.hpp"

/*
 * @param source: An address to a string object, needs to be alive when calling
 * @returns id: an unsigned integer which is the id of the shader.
 */
static uint32_t CompileShader(uint32_t type, const std::string& source) {
    uint32_t id = glCreateShader(type);
    const char* src = source.c_str();

    // from documentation: if length is null, src is assumed to be nullterminated.
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (!result) {

        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // stack allocation.
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!\n";
        std::cerr << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static uint32_t createShader(const std::string& vertexShader, const std::string& fragmentShader) {

    uint32_t program = glCreateProgram();
    uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    glfwSwapInterval(1);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    {
        float vectors[12] = {
            -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,

            -0.5f, 0.5,

        };

        uint32_t indices[] = {0, 1, 2, 2, 3, 0};

        uint32_t vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexBuffer vb(vectors, 4 * 2 * sizeof(float));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

        IndexBuffer ib(indices, 6);

        ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
        std::cout << "VERTEX\n";
        std::cout << source.VertexSource << '\n';

        std::cout << "FRAGMENT\n";
        std::cout << source.FragmentSource << '\n';

        uint32_t shader = createShader(source.VertexSource, source.FragmentSource);

        GLCall(glUseProgram(shader));
        GLCall(int location = glGetUniformLocation(shader, "u_color"));
        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float incr = 0.005f;
        while (!glfwWindowShouldClose(window)) {

            /* any rendering happens after this */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            GLCall(glBindVertexArray(vao));
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                incr = -0.005f;
            else if (r < 0.0f)
                incr = 0.005f;
            r += incr;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}
