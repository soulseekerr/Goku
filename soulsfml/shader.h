#pragma once

#include <format>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace soul {

class ShaderException : std::exception {
    const char* msg;
public:
    ShaderException() = delete;
    explicit ShaderException(const char* p_msg) noexcept : msg(p_msg) {}
    const char* what() const noexcept { return msg; } 
}; 

class Shader {
private:
    const std::string shaderPath = "/Users/soulseeker/Projects/GitHub/gokugame/source/shaders";
    // SFML Shader
    sf::Shader shader;
    bool isShaderLoaded = false;

public:
    Shader() {}
    virtual ~Shader() {}

    bool isLoaded() const;

    bool load(const std::string& shader_name);
};

} // namespace soul