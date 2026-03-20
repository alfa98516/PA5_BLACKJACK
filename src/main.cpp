#include <cassert>
#include <ios>
#ifdef __linux__
#include <alloca.h>
#include <signal.h>
#include <unistd.h>
#define ASSERT(x)                                                                                  \
    if (!(x))                                                                                      \
        raise(SIGTRAP);
#endif
#ifdef __APPLE__
#include <alloca.h>
#include <mach-o/dyld.h>
#define ASSERT(x)                                                                                  \
    if (!(x))                                                                                      \
        raise(SIGTRAP);
#endif
#ifdef _WIN32
#include <malloc.h>
#include <windows.h>
#define ASSERT(x)                                                                                  \
    if (!(x))                                                                                      \
        __debugbreak();

#endif
#define GLCall(x)                                                                                  \
    GLClearError();                                                                                \
    x;                                                                                             \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <glad/gl.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#define GLFW_INCLUDE_NONE // tells GLFW not to include any OpenGL headers itself
#include <GLFW/glfw3.h>
#include <iostream>

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << std::hex << error << ")" << " in: " << function << " "
                  << file << ":" << std::dec << line << std::endl;
        return false;
    }
    return true;
}

static std::filesystem::path getExecutableDir() {
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    return std::filesystem::path(path).parent_path();
#elif __linux__
    return std::filesystem::canonical("/proc/self/exe").parent_path();
#elif __APPLE__
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    _NSGetExecutablePath(path, &size);
    return std::filesystem::canonical(path).parent_path();
#endif
}

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& file) {

    std::filesystem::path fullPath = getExecutableDir().parent_path() / file;
    std::ifstream stream(fullPath);

    enum class ShaderType { NONE = -1, VERTEX, FRAGMENT };

    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream strstr[2];
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;

            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            strstr[(int)type] << line << '\n';
        }
    }
    return {strstr[0].str(), strstr[1].str()};
}

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

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    float vectors[12] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,

        -0.5f, 0.5,

    };

    uint32_t indices[] = {0, 1, 2, 2, 3, 0};

    uint32_t buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), vectors, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    uint32_t ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(uint32_t), indices, GL_STATIC_DRAW);

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

    float r = 0.0f;
    float incr = 0.5f;
    while (!glfwWindowShouldClose(window)) {
        /* any rendering happens after this */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
            incr = -0.5f;
        else if (r < 0.0f)
            incr = 0.5f;
        r += incr;

        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
