#pragma once

#include <filesystem>
#include <format>
#include <cmath>

#include "utils.h"
#include "entity.h"
#include "vector2.h"
#include "color.h"
#include "image.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace fs = std::filesystem;

namespace soul {

class TextureException : std::exception {
    const char* msg;
public:
    TextureException() = delete;
    explicit TextureException(const char* p_msg) noexcept : msg(p_msg) {}
    const char* what() const noexcept { return msg; } 
};

class Texture2d : public Entity {
private:
    // SFML texture
    sf::Texture texture;

    std::string _pathFile;
    bool _isSmooth;
    soul::Vector2f _scale;

public:
    Texture2d() = delete;    

    explicit Texture2d(
        const std::string& name, 
        const std::string& filePath, 
        bool isSmooth=true);

    virtual ~Texture2d();

    // Update the entity with specialized logic
    virtual bool update(float dt) { return isActive(); }

    virtual void updateData(int id) {}

    sf::Texture& getTexture();

    Response loadTexture(const std::string& filePath);

    Response loadTextureFromImage(
        const std::string& filePath, 
        const soul::Color& bgColor);

    // Helper function to check if two colors are "close enough" with a tolerance
    bool colorsAreClose(
        const soul::Color& color1, 
        const soul::Color& color2, 
        int tolerance);

    void makeTransparent(
        soul::Image& image, 
        const soul::Color& bgColor, 
        int tolerance);
};

inline sf::Texture& Texture2d::getTexture() { return texture; }

} // namespace soul