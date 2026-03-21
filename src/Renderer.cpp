#include "Renderer.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

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
