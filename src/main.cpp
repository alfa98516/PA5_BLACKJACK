#include <alloca.h>
#include <cstdint>
#include <glad/gl.h>
#include <string>
#include <sys/types.h>
#define GLFW_INCLUDE_NONE // tells GLFW not to include any OpenGL headers itself
#include <GLFW/glfw3.h>
#include <iostream>

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
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fagment")
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // gladLoadGL replaces glewInit
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    float vectors[6]{-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5};

    uint32_t buffer;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vectors, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader = "#version 330 core\n"
                               "\n"
                               "layout(location = 0) in vec4 position;\n"
                               "\n"
                               "void main() {\n"
                               "   gl_Position = position;\n"
                               "}\n";

    std::string fragmentShader = "#version 330 core\n"
                                 "layout(location = 0) out vec4 color;"
                                 "\n"
                                 "void main() {\n"
                                 "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
                                 "}\n";

    uint32_t shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        /* any rendering happens after this */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(shader);

    glfwTerminate();
    return 0;
}
