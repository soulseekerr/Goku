#pragma once

#include <filesystem>
#include <format>
#include <math.h>

#include "core/vector2.h"
#include "core/rect.h"
#include "color.h"
#include "texture2d.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace fs = std::filesystem;

namespace soul {

class ShaderException : std::exception {
    const char* msg;
public:
    ShaderException() = delete;
    explicit ShaderException(const char* p_msg) noexcept : msg(p_msg) {}
    const char* what() const noexcept { return msg; } 
}; 

struct sSpriteData {
    // Filename for the image file
    std::string filename;
    // Scaling the sprite from the image
    soul::Vector2f spriteScale;
    // Initial px position in the Sprite sheet
    soul::Vector2i initialSpritePosition;
    // Initial px size in the Sprite sheet
    soul::Vector2i initialSpriteSize;
    // Coordinates px of screen as they are defined 0,0 from top left.
    soul::Vector2i initialPosition;
    // Filter Transparency By Color
    bool enableColorFilter {false};
    // Filtering Color
    soul::Color filterColor;
};

class Scene;

class Sprite2d {
private:
    // Sprite name
    std::string _name;
    // Texture on the sprite
    std::shared_ptr<soul::Texture2d> _texture;

    // SFML Sprite
    sf::Sprite _sprite;

public:
    // Position of the sprite
    soul::Vector2f position;
    // Scale of the sprite
    soul::Vector2f scale;

    Sprite2d() = delete;
    explicit Sprite2d(const std::string& name) : _name(name){}
    virtual ~Sprite2d() {}

    sf::Sprite& getSprite() { return _sprite; }

    std::shared_ptr<soul::Texture2d> getTexture() { return _texture; }

    void loadTexture(const std::string& filePath, const soul::Vector2f& scale, bool isSmooth);
    void loadTextureImageFilter(const std::string& filePath, const soul::Vector2f& scale, bool isSmooth, soul::Color bgColor);

    void setTextureRect(const soul::Vector2i& pos, const soul::Vector2i& size);
    void setScale(float fx, float fy);
    void setScaleFactor(int fx, int fy);
    void setPosition(float x, float y);
};

} // namespace soul