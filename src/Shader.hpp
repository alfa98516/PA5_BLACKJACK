#pragma once
#include <cstdint>
#include <filesystem>
#include <string>
#include <sys/types.h>
#include <unordered_map>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
  private:
    uint32_t m_RendererID;
    std::string m_Filepath;
    std::unordered_map<std::string, int32_t> m_UniformLocationCache;

    uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

    int32_t GetUniformLocation(const std::string& name);

    uint32_t CompileShader(uint32_t type, const std::string& source);

    ShaderProgramSource ParseShader(const std::string& file);

  public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // set uniforms

    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform1i(const std::string& name, int32_t value);
};

std::filesystem::path getExecutableDir();
