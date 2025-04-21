
#include "tile.h"
#include "gamewindow.h"

using namespace soul;

// Load the sprite data
void RigidBody::load(const sSpriteData& spriteData) {
    // Initial parameters
    _sprite->position.x = spriteData.initialPosition.x;
    _sprite->position.y = spriteData.initialPosition.y;
    _sprite->velocity = Vector2f(0.0f, 0.0f);
    _sprite->scale.x = spriteData.spriteScale.x;
    _sprite->scale.y = spriteData.spriteScale.y;

    _sprite->transform.initialVelocityX = 0.0f;
    _sprite->transform.gravity = 0.0f;
    _sprite->transform.initialVelocityY = 0.0f;

    logManager.log("Loading Texture: {}", spriteData.filename);
    try {
        if (spriteData.enableColorFilter) {

            _sprite->loadTextureImageFilter(
                spriteData.filename, 
                Vector2f(_sprite->scale.x, _sprite->scale.y), 
                true,
                spriteData.filterColor);

        } else {

            _sprite->loadTexture(
                spriteData.filename, 
                Vector2f(_sprite->scale.x, _sprite->scale.y), 
                true);
        }
    }
    catch(TextureException& e) {
        logManager.logError("Error occurred while loading Texture: {}\n", e.what());
        return;
    }

    _sprite->setTextureRect(
        spriteData.initialSpritePosition.x,
        spriteData.initialSpritePosition.y,
        spriteData.initialSpriteSize.x,
        spriteData.initialSpriteSize.y);

    // inverted position on y axis
    _sprite->setPosition(
        _sprite->position.x,
        _groundY - _sprite->position.y);

    _sprite->setRotation(0.0f);
}

bool Tile::update(float dt) {
    return true;
}

void Tile::render() {
    soul::GameWindow& gw = soul::GameWindow::getInstance();
    gw.window.draw(_sprite->getSprite());
}