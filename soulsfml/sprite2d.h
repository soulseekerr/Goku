#pragma once

#include <filesystem>
#include <format>
#include <math.h>

#include "core.h"
#include "vector2.h"
#include "rect.h"
#include "color.h"
#include "texture2d.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace fs = std::filesystem;

namespace soul {

/**
 * @brief Sprite Data
 *  Contains the data for the sprite
 * */
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
    soul::Vector2f initialPosition;
    // Filter Transparency By Color
    bool enableColorFilter {false};
    // Filtering Color
    soul::Color filterColor;
};

class Scene;

/** 
 * @brief Transform Scalars
 * Contains the data defined for the sprite dynamics
*/
struct sTransformScalars {
    float initialVelocityX {0.0f};
    float initialVelocityY {0.0f};
    float gravity {0.0f};
    int direction {1};
    float angle {0.0f};
    int groundY {800};
};

/**
 * @brief Sprite2d
 * It is a 2D sprite class that can be used to render 2D images on the screen
 */
class Sprite2d {
private:
    // Sprite name
    std::string _name;
    // Texture on the sprite
    std::shared_ptr<soul::Texture2d> _texture;
    // SFML Sprite
    std::shared_ptr<sf::Sprite> _sprite;

public:
    // Position of the sprite
    soul::Vector2f position;
    // Scale of the sprite
    soul::Vector2f scale;
    // Speed of the sprite
    soul::Vector2f velocity;

    sTransformScalars transform;

    Sprite2d() = delete;
    explicit Sprite2d(const std::string& name) : _name(name){}
    virtual ~Sprite2d() {}

    _ALWAYS_INLINE_ sf::Sprite& getSprite() { 
        return *_sprite.get(); 
    }

    _ALWAYS_INLINE_ std::shared_ptr<soul::Texture2d> getTexture() { 
        return _texture; 
    }

    void loadTexture(const std::string& filePath, const soul::Vector2f& scale, bool isSmooth);

    void loadTextureImageFilter(const std::string& filePath, const soul::Vector2f& scale, bool isSmooth, soul::Color bgColor);

    // void setTextureRect(const soul::Vector2i& pos, const soul::Vector2i& size);
    void setTextureRect(const int& px, const int& py, const int& sx, const int& sy);
    
    void setScale(float fx, float fy);
    
    void setScaleFactor(int fx, int fy);
    
    void setPosition(float x, float y);
    
    void setVelocity(float x, float y);

    void setRotation(float angle);
};

} // namespace soul