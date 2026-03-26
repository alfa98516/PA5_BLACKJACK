#include "Shader.hpp"
#include "Macros.hpp"
#include <cstdint>
#include <fstream>
#include <glad/gl.h>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string& filepath) : m_Filepath(filepath) {

    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}
Shader::~Shader() { GLCall(glDeleteProgram(m_RendererID)); }

void Shader::Bind() const { GLCall(glUseProgram(m_RendererID)); }

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, float value) {

    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, int32_t value) {

    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int32_t Shader::GetUniformLocation(const std::string& name) {

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    GLCall(uint32_t location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist\n";
    } else {
        m_UniformLocationCache[name] = location;
    }

    return location;
}

void Shader::Unbind() const { GLCall(glUseProgram(0)); }

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

ShaderProgramSource Shader::ParseShader(const std::string& file) {

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

uint32_t Shader::CompileShader(uint32_t type, const std::string& source) {
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

uint32_t Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

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
