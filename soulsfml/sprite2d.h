#pragma once

#include <filesystem>
#include <format>
#include <math.h>

#include "core/vector2.h"
#include "color.h"
#include "texture2d.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace fs = std::filesystem;

namespace soul {

class Scene;

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
    // Initial position in the Sprite sheet
    soul::Vector2i initialSpritePosition;
    // Initial size in the Sprite sheet
    soul::Vector2i initialSpriteSize;
    // Coordinates of screen as they are defined 0,0 from top left.
    soul::Vector2i initialPosition;
    // Filter Transparency By Color
    bool enableColorFilter {false};
    // Filtering Color
    soul::Color filterColor;
};

class Sprite2d {
private:
    // Sprite name
    std::string _name;
    // SFML Sprite
    sf::Sprite _sprite;
    // Texture on the sprite
    std::shared_ptr<soul::Texture2d> _texture;
    // Scale applied on the sprite
    soul::Vector2f _scale;

public:
    soul::Vector2f position;

    Sprite2d() = delete;
    explicit Sprite2d(const std::string& name) : _name(name){}
    virtual ~Sprite2d() {}

    sf::Sprite& getSprite() { return _sprite; }
    soul::Texture2d& getTextureRef() { return *_texture.get(); }
    soul::Vector2f& getScale() { return _scale; }

    void loadTexture(const std::string& p_file_path, const soul::Vector2f& p_scale, bool is_smooth);
    void loadTextureImageFilter(const std::string& p_file_path, const soul::Vector2f& p_scale, bool is_smooth, soul::Color backgroundColor);

    void setTextureRect(const soul::Vector2i& pos, const soul::Vector2i& size);
    void setScale(int fx, int fy);
    void setScaleFactor(int fx, int fy);
    void setPosition(float x, float y);
};

class Physics {
public:
    static bool isAABBIntersection(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {

        // Check if there is no overlap in the horizontal axis
        if (rect1.left + rect1.width <= rect2.left || rect2.left + rect2.width <= rect1.left) {
            return false;
        }
        
        // Check if there is no overlap in the vertical axis
        if (rect1.top + rect1.height <= rect2.top || rect2.top + rect2.height <= rect1.top) {
            return false;
        }
        
        // If neither condition is true, then the rectangles are intersecting
        return true;
    }
};

} // namespace soul