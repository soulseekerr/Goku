
#include "sprite2d.h"
#include "assetmanager.h"

using namespace soul;

void Sprite2d::loadTexture(
    const std::string& p_file_path, 
    const soul::Vector2f& p_scale, 
    bool is_smooth) {

    auto& mgr = AssetManager::getInstance();
    _texture = mgr.addTexture(std::format("TEX_{}", _name), p_file_path, is_smooth);
    _sprite.setTexture(_texture->getTexture());
    _scale = p_scale;
    _sprite.setScale(_scale.x, _scale.y);

    // sf::FloatRect spriteSize = sprite.getGlobalBounds();
    // sprite.setOrigin(spriteSize.width/2., spriteSize.height/2.);
}

void Sprite2d::loadTextureImageFilter(
    const std::string& p_file_path, 
    const soul::Vector2f& p_scale, 
    bool is_smooth, 
    soul::Color backgroundColor) {

    _texture = AssetManager::getInstance().addTextureImageFilter(std::format("TEX_{}", _name), p_file_path, is_smooth, backgroundColor);
    _sprite.setTexture(_texture->getTexture());

    _scale = p_scale;
    _sprite.setScale(_scale.x, _scale.y);

    // sf::FloatRect spriteSize = sprite.getGlobalBounds();
    // sprite.setOrigin(spriteSize.width/2., spriteSize.height/2.);
}

void Sprite2d::setTextureRect(const soul::Vector2i& pos, const soul::Vector2i& size) {
    _sprite.setTextureRect(sf::IntRect(pos.x, pos.y, size.x, size.y));
}

void Sprite2d::setScale(int fx, int fy) {
    _sprite.setScale(fx, fy);
}

void Sprite2d::setScaleFactor(int fx, int fy) {
    _sprite.setScale(fx*_scale.x, fy*_scale.y);
}

void Sprite2d::setPosition(float x, float y) {
    _sprite.setPosition(x, y);
}
