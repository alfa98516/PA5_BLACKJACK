#pragma once
#include <filesystem>
#include <sys/types.h>
#ifndef NO_GL
#define GLFW_INCLUDE_NONE // tells GLFW not to include any OpenGL headers itself
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#endif
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

constexpr bool DEBUG = true;
constexpr int WINDOW_WIDTH = 960;
constexpr int WINDOW_HEIGHT = 540;

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
