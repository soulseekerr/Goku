
#include "animable.h"
#include "sprite2d.h"

using namespace soul;

Animable::Animable(const std::string& name) : Entity(name) {
    _sprite = std::make_shared<Sprite2d>(this->tag());
    _animations = std::make_shared<AnimationSet>(_sprite);
}

void Animable::load(const int& windowPositionY, const sSpriteData& spriteData, const sTransformScalars& scalars) {

    _windowPositionY = windowPositionY;

    _sprite->position.x = spriteData.initialPosition.x;
    _sprite->position.y = spriteData.initialPosition.y;
    _initialPositionY = spriteData.initialPosition.y;
    _sprite->velocity = Vector2f(0.0f, 0.0f);
    _sprite->scale.x = spriteData.spriteScale.x;
    _sprite->scale.y = spriteData.spriteScale.y;
    
    
    _sprite->transform.moveSpeed = scalars.moveSpeed;
    _sprite->transform.gravityForce = scalars.gravityForce;
    _sprite->transform.jumpForce = scalars.jumpForce;
 
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
        exit(-1);
    }

    _sprite->setTextureRect(spriteData.initialSpritePosition, spriteData.initialSpriteSize);

    // inverted position on y axis
    _sprite->setPosition(_sprite->position.x, _windowPositionY - _sprite->position.y);
}

void Animable::addAnimation(AnimationState state, std::shared_ptr<SpriteAnimation> animation) {
    _animations->addAnimation(state, animation);
}

void Animable::handleInput() {
    if (!_currentMovementState) return;

    _currentMovementState->handleInput(*this);
}

void Animable::updateStates(float dt) {
    if (_currentMovementState) {
        _currentMovementState->update(*this, dt);
    }
    _animations->update(dt);
}

void Animable::setAnimationState(AnimationState state) {
    _animations->setAnimationState(state);
}
    
void Animable::setMovementState(MovementState& state) {
    _currentMovementState = &state;
    _currentMovementState->enter(*this);
}