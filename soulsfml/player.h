#pragma once

#include "core.h"
#include "animable.h"
#include "fireball.h"

#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

namespace soul {

struct PlayerAttributes {
    int currentHealth = 1;
    int maxHealth = 1;
    int currentMana = 1;
    int maxMana = 1;
    int currentExp = 1;
    int maxExp = 1;
    int level = 1;
};

class Player : public Animable {
    private:
    // System of Fireballs shot by the Player
    FireballSystem _fireballs;
    // Player attributes
    PlayerAttributes _attributes;

public:
    Player() = delete;
    explicit Player(const std::string& name);
    virtual ~Player() = default;

    // Update the entity with specialized logic
    virtual bool update(float dt) override;

    virtual void render() override;

    _ALWAYS_INLINE_ PlayerAttributes& getAttributes() { return _attributes; }

    void setAttributes(const PlayerAttributes& attributes) {
        _attributes = attributes;
    }

    _ALWAYS_INLINE_ FireballSystem& getFireballSystem() { return _fireballs; }

    void initFireballs(const std::string& filePath, float speedX, float lifetime);
};

} // namespace soul