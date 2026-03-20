#include "Renderer.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << std::hex << error << ")" << " in: " << function << " "
                  << file << ":" << std::dec << line << std::endl;
        return false;
    }
    return true;
}

void GLClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

std::filesystem::path getExecutableDir() {
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

ShaderProgramSource ParseShader(const std::string& file) {

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
