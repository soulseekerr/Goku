#pragma once

#include "core.h"
#include "vector2.h"
#include "entity.h"
#include "sprite2d.h"

#include <SFML/Graphics.hpp>

namespace soul {

class Sprite2d;
struct sSpriteData;

class RigidBody : public Entity {
protected:
    // Entity has a sprite instance
    std::shared_ptr<Sprite2d> _sprite;
    bool isColliding_ {false};
    // Ajustment to invert the vertical position of the sprite in the window
    int _groundY;

public:
    RigidBody() = delete;
    explicit RigidBody(const std::string& p_tag, const sSpriteData& spriteData, bool isColliding) : Entity(p_tag), isColliding_(isColliding) {
        _sprite = std::make_shared<Sprite2d>(this->tag());
        _groundY = 575;
        load(spriteData);
    }
    virtual ~RigidBody() = default;

    // Load the sprite data
    void load(const sSpriteData& spriteData);

    // Update the entity with specialized logic
    virtual bool update(float dt) override { return true; }

    virtual void render() override {}

    // Get methods to access the sprite and animations
    _ALWAYS_INLINE_ sf::Sprite& getSprite() const {
        return _sprite->getSprite();
    }

    _ALWAYS_INLINE_ const soul::Vector2f& getPosition() const {
        return _sprite->position;
    }

    // _ALWAYS_INLINE_ const sTransformScalars& getTransform() const {
    //     return _sprite->transform;
    // }

    _ALWAYS_INLINE_ void setPosition(float x, float y) {
        _sprite->setPosition(x, y);        
    }

    _ALWAYS_INLINE_ const int& getGroundY() const {
        return _groundY;
    }
};

class Tile : public RigidBody {
public:
    Tile() = delete;
    explicit Tile(const std::string& p_tag, const sSpriteData& spriteData, bool isColliding = false) : RigidBody(p_tag, spriteData, isColliding) {}
    virtual ~Tile() = default;

    // Update the entity with specialized logic
    virtual bool update(float dt) override;

    virtual void render() override;
};

} // namespace soul