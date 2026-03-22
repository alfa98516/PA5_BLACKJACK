#pragma once
#include <sys/types.h>
#define GLFW_INCLUDE_NONE // tells GLFW not to include any OpenGL headers itself
#include <GLFW/glfw3.h>
#include <glad/gl.h>
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
#include <iostream>

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << std::hex << error << ")" << " in: " << function << " "
                  << file << ":" << std::dec << line << std::endl;
        return false;
    }
    return true;
}

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}
