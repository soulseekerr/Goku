#pragma once

#include "animable.h"
#include "spriteanimation.h"
#include "entitymanager.h"

namespace soul {

class Player;

struct sFireballMetrics {
    float   speedX {250.0f};
    float   lifetime {1.0f};
    int     direction {1};
};

/**
 * @brief Fireball class
 * Class that represents a fireball entity
 */
class Fireball : public Animable {
private:
    Player& _playerRef;
    sFireballMetrics _metrics;
    soul::Vector2i _initialPosition;
    float _angleSprite;

public:
    Fireball() = delete;

    explicit Fireball(
        Player& player,
        const std::string& tag, 
        int initialPosX, 
        int initialPosY, 
        float initialLifetime, 
        float speedX, 
        int direction,
        float angle,
        bool active = true);

    virtual ~Fireball() = default;

    // Load the entity with specialized logic
    virtual void loadData() override;

    // Update the entity with specialized logic
    virtual bool update(float dt) override;

    const float& getSpeedX() const { return _metrics.speedX; }
    const float& getLifetime() const { return _metrics.lifetime; }
};

} // namespace soul