#pragma once

#include "core/core.h"
#include "animable.h"
#include "fireball.h"

#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

namespace soul {


class Player : public Animable {
public:
    Player() = delete;
    explicit Player(const std::string& name);
    virtual ~Player() = default;

    // Load the entity with specialized logic
    virtual void loadData() override;

    // Update the entity with specialized logic
    virtual bool update(float dt) override;

    virtual void updateData(int id) override {}

    virtual void render() override;

private:
    const std::string _filePathFireball = "/Users/soulseeker/Projects/GitHub/gokugame/debug/textures/fireballs.png";    
    // System of Fireballs shot by the Player
    FireballSystem _fireballs;
};

} // namespace soul