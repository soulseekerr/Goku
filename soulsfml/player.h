#pragma once

#include "core/core.h"
#include "animable.h"

#include <string>
#include <memory>

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

    void spawnFireball();
};

} // namespace soul