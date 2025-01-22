#pragma once

#include "ecs/entity.h"
#include "spriteanimation.h"
#include "state.h"

#include <string>
#include <memory>

namespace soul {

class Sprite2d;
struct sSpriteData;

struct sInput {
    bool up {false};
    bool down {false};
    bool left {false};
    bool right {false};

    bool punch {false};
    bool punchStick {false};
    bool kick {false};
    bool kick2 {false};
    bool jumpkick {false};
    bool shoot {false};
    bool knocked {false};
    bool defensive {false};

    bool is_shooting {false};
    bool is_jumping {false};
    bool is_on_ground {false};
};

class Animable : public Entity {
    friend class MovementState;
protected:
    // Renderable has a sprite instance
    std::shared_ptr<Sprite2d> _sprite;
    // List of animations for the player from the sprite (sheet)
    std::shared_ptr<AnimationSet> _animations;
    // Ajustment to invert the vertical position of the sprite in the window
    int _windowPositionY;

    int _initialPositionY;

    MovementState* _currentMovementState {nullptr};

public:
    // Input states
    sInput input;

public:
    Animable() = delete;
    explicit Animable(const std::string& name);
    virtual ~Animable() = default;

    sf::Sprite& getSprite() const { return _sprite->getSprite();}

    std::shared_ptr<AnimationSet>& getAnimations() { return _animations; }

    void load(const int& windowPositionY, const sSpriteData& spriteData, const sTransformScalars& scalars);

    void addAnimation(AnimationState state, std::shared_ptr<SpriteAnimation> animation);

    void handleInput();

    void updateStates(float dt);

    void setAnimationState(AnimationState state);
    
    void setMovementState(MovementState& state);

    void incrPositionX(float x) {
        _sprite->position.x += x;
    }

    void incrPositionY(float y) {
        _sprite->position.y += y;
    }

    void setVelocityX(float x) {
        _sprite->velocity.x = x;
    }

    void setPosition(float x, float y) {
        _sprite->setPosition(x, y);
    }

    void resetPosition() {
        // inverted position on y axis
        setPosition(_sprite->position.x, _windowPositionY - _sprite->position.y);
    }
};

} // namespace soul