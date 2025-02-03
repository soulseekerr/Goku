#pragma once

#include "core/core.h"
#include "ecs/entity.h"
#include "spriteanimation.h"
#include "state.h"

#include <string>
#include <memory>

namespace soul {

class Sprite2d;
struct sSpriteData;

// Input that the player can do and has an effect on the states
struct sInput {
    // Movements
    bool up {false};
    bool down {false};
    bool left {false};
    bool right {false};

    // Actions
    bool punch {false};
    // bool punchStick {false};
    // bool kick {false};
    // bool kick2 {false};
    // bool jumpkick {false};
    bool shoot {false};
    // bool knocked {false};
    // bool defensive {false};

    // Boolean states
    bool is_shooting {false};
    bool is_jumping {false};
    bool is_on_ground {false};
};

/**
 * @brief Animable class
 * Class that embeds a sprite and animations for an active entity
 */
class Animable : public Entity {
    // TOCHANGE: friend class to allow access to the sprite instance
    friend class MovementState;
private:
    // Entity has a sprite instance
    std::shared_ptr<Sprite2d> _sprite;
    // List of animations for the player from the sprite (sheet)
    std::shared_ptr<AnimationSet> _animations;
    // Ajustment to invert the vertical position of the sprite in the window
    int _groundY;
    // Current State of the entity
    MovementState* _currentMovementState {nullptr};
    // Current Action of the entity
    ActionState* _currentActionState {nullptr};

public:
    // Input states
    sInput input;

public:
    Animable() = delete;
    explicit Animable(const std::string& name);
    virtual ~Animable() = default;

    // Get methods to access the sprite and animations
    _ALWAYS_INLINE_ sf::Sprite& getSprite() const {
        return _sprite->getSprite();}
    
    _ALWAYS_INLINE_ std::shared_ptr<AnimationSet>& getAnimations() { 
        return _animations;
    }
    
    _ALWAYS_INLINE_ const soul::Vector2f& getPosition() const {
        return _sprite->position;
    }

    _ALWAYS_INLINE_ const sTransformScalars& getTransform() const {
        return _sprite->transform;
    }

    _ALWAYS_INLINE_ const int& getGroundY() const {
        return _groundY;
    }

    _ALWAYS_INLINE_ void incrPositionX(float x) {
        _sprite->position.x += x;
    }

    _ALWAYS_INLINE_ void incrPositionY(float y) {
        _sprite->position.y += y;
    }

    _ALWAYS_INLINE_ void setVelocityX(float x) {
        _sprite->velocity.x = x;
    }

    _ALWAYS_INLINE_ void setPosition(float x, float y) {
        _sprite->setPosition(x, y);        
    }

    _ALWAYS_INLINE_ void resetPosition() {
        _sprite->setPosition(_sprite->position.x, _sprite->position.y);
    }

    // Load the sprite data and the transform scalars
    void load(const sSpriteData& spriteData, const sTransformScalars& scalars);

    // Add an animation to the animation set
    void addAnimation(AnimationState state, std::shared_ptr<SpriteAnimation> animation);

    // Handle the input from the animable object
    void handleInput();

    // Update the states of the animable object
    void updateStates(float dt);

    // Set the current animation state
    void setAnimationState(AnimationState state);

    // Set the current movement state    
    void setMovementState(MovementState& state);

    // Set current action state
    void setActionState(ActionState& state);
};

} // namespace soul