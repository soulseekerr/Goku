#pragma once

#include <filesystem>
#include <format>
#include <cmath>

#include "core/utils.h"
#include "ecs/entity.h"
#include "core/vector2.h"

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

    string _pathFile;
    bool _isSmooth;
    soul::Vector2f _scale;

public:
    Texture2d() = delete;    
    explicit Texture2d(const std::string& name, const std::string& pathFile, bool isSmooth=true);
    virtual ~Texture2d();

    sf::Texture& getTexture();

    Response loadTexture(const string& p_pathfile);
    Response loadTextureFromImage(const std::string& p_pathfile, sf::Color backgroundColor);

    // Helper function to check if two colors are "close enough" with a tolerance
    bool colorsAreClose(const sf::Color& color1, const sf::Color& color2, int tolerance);

    void makeTransparent(sf::Image &image, sf::Color backgroundColor, int tolerance);
};

inline sf::Texture& Texture2d::getTexture() { return texture; }

} // namespace soul