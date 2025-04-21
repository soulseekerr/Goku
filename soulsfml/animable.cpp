
#include "animable.h"
#include "sprite2d.h"
#include "gamewindow.h"

using namespace soul;

/**
 * @brief Construct a new Animable:: Animable object
 * Animable is an entity that has a sprite and animations
 */
Animable::Animable(const std::string& name) : Entity(name) {
    _sprite = std::make_shared<Sprite2d>(this->tag());
    _animations = std::make_shared<AnimationSet>(_sprite);
}

void Animable::load(const sSpriteData& spriteData, const sTransformScalars& scalars) {

    // Position Y for the ground
    _groundY = scalars.groundY;

    // Initial parameters
    _sprite->position.x = spriteData.initialPosition.x;
    _sprite->position.y = spriteData.initialPosition.y;
    _sprite->velocity = Vector2f(0.0f, 0.0f);
    _sprite->scale.x = spriteData.spriteScale.x;
    _sprite->scale.y = spriteData.spriteScale.y;

    _sprite->transform.initialVelocityX = scalars.initialVelocityX;
    _sprite->transform.gravity = scalars.gravity;
    _sprite->transform.initialVelocityY = scalars.initialVelocityY;
 
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

    _sprite->setTextureRect(
        spriteData.initialSpritePosition.x, spriteData.initialSpritePosition.y,
        spriteData.initialSpriteSize.x, spriteData.initialSpriteSize.y);

    // inverted position on y axis
    _sprite->setPosition(_sprite->position.x, _groundY - _sprite->position.y);

    _sprite->setRotation(scalars.angle);
}

void Animable::addAnimation(AnimationState state, std::shared_ptr<SpriteAnimation> animation) {
    _animations->addAnimation(state, animation);
}

// Handle the input from the animable object
void Animable::handleInput() {
    if (_currentMovementState) {
        _currentMovementState->handleInput(*this);
    }

    if (_currentActionState) {
        _currentActionState->handleInput(*this);
    }
}

// Update the states of the animable object
void Animable::updateStates(float dt) {
    if (_currentMovementState) {
        _currentMovementState->update(*this, dt);
    }

    if (_currentActionState) {
        _currentActionState->update(*this, dt);
    }

    _animations->update(dt);
}

// Set the animation state
void Animable::setAnimationState(AnimationState state) {
    _animations->setAnimationState(state);
}

// Set the current movement state 
void Animable::setMovementState(MovementState& state) {
    _currentMovementState = &state;
    _currentMovementState->enter(*this);
}

// Set the current action state 
void Animable::setActionState(ActionState& state) {
    _currentActionState = &state;
    _currentActionState->enter(*this);
}

bool Animable::update(float dt) {

    handleInput();
    
    // Update the states and animation associated to the current movement or action
    updateStates(dt);

    return isActive();
}

void Animable::render() {
    soul::GameWindow& gw = soul::GameWindow::getInstance();

    // std::cout << "Render " << this->ID() << std::endl;

    gw.window.draw(_sprite->getSprite());
}