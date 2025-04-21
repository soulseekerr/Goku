#pragma once

#include "gamewindow.h"

#include <format>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace soul {

/**
 * @brief Exception class for Shader
 */
class ShaderException : std::exception {
    const char* msg;
public:
    ShaderException() = delete;
    explicit ShaderException(const char* p_msg) noexcept : msg(p_msg) {}
    const char* what() const noexcept { return msg; } 
}; 

/**
 * @brief Shader class
 * It is a class that represents a shader
 */
class Shader {
public:
    // SFML Shader
    sf::Shader shader;
    bool isShaderLoaded = false;

public:
    Shader() {}
    virtual ~Shader() {}

    bool isLoaded() const;

    bool load(const std::string& shader_name);
};

class FireballShader : public Shader {
private:
    // Game Window
    GameWindow& gw = soul::GameWindow::getInstance();

public:
    FireballShader() {}
    virtual ~FireballShader() {}

    void update(float dt) {
        // shader.setUniform("time", clock.getElapsedTime().asSeconds());
        shader.setUniform("time", dt);
        shader.setUniform("resolution", sf::Vector2f(gw.window.getSize()));
        shader.setUniform("texture0", sf::Shader::CurrentTexture);
    }
};

} // namespace soul