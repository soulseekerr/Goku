
#include "sprite2d.h"
#include "assetmanager.h"

using namespace soul;

void Sprite2d::loadTexture(
    const std::string& filePath, 
    const soul::Vector2f& scale, 
    bool isSmooth) {

    auto& mgr = AssetManager::getInstance();
    _texture = mgr.addTexture(std::format("TEX_{}", _name), filePath, isSmooth);
    _sprite.setTexture(_texture->getTexture());

    this->scale = scale;
    _sprite.setScale(this->scale.x, this->scale.y);

    // sf::FloatRect spriteSize = sprite.getGlobalBounds();
    // sprite.setOrigin(spriteSize.width/2., spriteSize.height/2.);
}

void Sprite2d::loadTextureImageFilter(
    const std::string& filePath, 
    const soul::Vector2f& scale, 
    bool isSmooth, 
    soul::Color bgColor) {

    _texture = AssetManager::getInstance().addTextureImageFilter(std::format("TEX_{}", _name), filePath, isSmooth, bgColor);
    _sprite.setTexture(_texture->getTexture());

    this->scale = scale;
    _sprite.setScale(this->scale.x, this->scale.y);

    // sf::FloatRect spriteSize = sprite.getGlobalBounds();
    // sprite.setOrigin(spriteSize.width/2., spriteSize.height/2.);
}

void Sprite2d::setTextureRect(const soul::Vector2i& pos, const soul::Vector2i& size) {
    _sprite.setTextureRect(sf::IntRect(pos.x, pos.y, size.x, size.y));
}

void Sprite2d::setScale(float fx, float fy) {
    _sprite.setScale(fx, fy);
}

void Sprite2d::setScaleFactor(int fx, int fy) {
    _sprite.setScale(fx*scale.x, fy*scale.y);
}

void Sprite2d::setPosition(float x, float y) {
    _sprite.setPosition(x, y);
}

void Sprite2d::setVelocity(float x, float y) {
    velocity.x = x;
    velocity.y = y;
}

void Sprite2d::setRotation(float angle) {
    _sprite.setRotation(angle);
}