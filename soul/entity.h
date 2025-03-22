#pragma once

#include "core.h"
#include "logger.h"

#include <iostream>
#include <random>
#include <ctime>
#include <memory>
#include <string>
#include <cstdint>
#include <sstream>

namespace soul {

// Random number generator class
class RandomNumberGenerator {
    uint32_t _min = 1;
    uint32_t _max = 1000000;
public:
    // Constructor: Seed the random number generator with the current time by default
    RandomNumberGenerator()
        : generator(static_cast<unsigned>(std::time(nullptr))),
          distribution(_min, _max) {}

    // Constructor: Allow custom seeding
    RandomNumberGenerator(unsigned seed) : generator(seed), distribution(_min, _max) {}

    // Generate a random integer in the range [min, max]
    uint32_t getID() {
        return distribution(generator);
    }

private:
    // Mersenne Twister 19937 generator and uniform distribution
    std::mt19937 generator; // Mersenne Twister 19937 generator
    std::uniform_int_distribution<uint32_t> distribution; // Distribution defined as a member variable
};

// Forward declaration of AScene
// class AScene;
class Entity;

struct IRenderable {
    // Load the entity with specialized logic
    virtual void loadData() = 0;
    // Render the entity with specialized logic
    virtual void render() = 0;
};

struct IUpdatable {
    // Update the entity with specialized logic
    [[nodiscard]] virtual bool update(float dt) = 0;
    // Update data
    virtual void updateData(int id) = 0;
};

/**
 * @brief Base class for all entities in the game world.
 * 
 * An entity is a game object that can be rendered, updated, and destroyed.
 * An entity can be a player, an enemy, a projectile, or any other object in the game world.
 * 
 * Each entity has a unique ID, a tag, and an active state.
 * The unique ID is used to identify the entity.
 * The tag is a string that describes the entity.
 * The active state determines if the entity is active in the game world.
 * 
 * The entity class is abstract and serves as the base class for all entities in the game world.
 * 
 * @note The entity class is not intended to be instantiated directly.
 */
class Entity : public IRenderable, IUpdatable {
private:
    static RandomNumberGenerator _gen;
    // Tag of the entity
    std::string _tag;
    // Unique ID of the entity
    uint32_t _id {0};
    // Active state of the entity
    bool _active {true};
    // Renderable is necessarily defined in a Scene
    // std::shared_ptr<AScene> _sceneRef;

protected:
    LoggerManager& logManager = LoggerManager::getInstance();

public:
    Entity() = delete;
    explicit Entity(const std::string& tag);
    virtual ~Entity() = default;

    const std::string& tag() const;
    const uint32_t& ID() const;

    // Check if the entity is active
    _ALWAYS_INLINE_ bool isActive() const { return _active; }

    void setActive(bool active);

    // Load the entity with specialized logic
    virtual void loadData() override {}

    // Update the entity with specialized logic
    virtual bool update(float dt) override { return _active; }

    virtual void updateData(int id) override {}

    virtual void render() override {}
};

} // namespace soul