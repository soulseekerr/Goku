#pragma once

#include "animable.h"
#include "spriteanimation.h"

#include <latch>

namespace soul {

class Player;

struct sFireballMetrics {
    float   speedX {250.0f};
    float   lifetime {1.0f};
    int     direction {1};
};

class FireballSystem;

/**
 * @brief Fireball class
 * Class that represents a fireball entity
 */
class Fireball : public Animable {
private:
    // Reference to the system of fireballs
    FireballSystem& _system;
    sFireballMetrics _metrics;
    soul::Vector2f _initialPosition;
    float _angleSprite;

public:
    Fireball() = delete;

    explicit Fireball(
        FireballSystem& system,
        const std::string& tag, 
        float initialPosX, 
        float initialPosY, 
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

    virtual void updateData(int id) override;

    const float& getSpeedX() const { return _metrics.speedX; }
    const float& getLifetime() const { return _metrics.lifetime; }
};

/**
 * @brief FireballSystem class
 * 
 */
class FireballSystem {
private:
    soul::LoggerManager& logManager = soul::LoggerManager::getInstance();
    
    // Pointer to the instance of Player
    Player* _player;
    // Current position and direction to save when we shoot
    soul::Vector2f _currentPosition;
    int _currentDirection;
    // Number of fireballs to shoot
    static constexpr int _thr_fireball_count = 2;
    // Container for poolables (flyweight)
    std::vector<std::shared_ptr<Entity>> _fireballs;
    // Atomic counter variable used as lock for fireball shots
    std::atomic<int> _thr_current_count_fireball{0};
        
public:
    FireballSystem() : _player(nullptr) {}
    ~FireballSystem() = default;

    const Player& getPlayer() const { return *_player; } 

    const soul::Vector2f& getCurrentPosition() const { return _currentPosition; }
    const int getCurrentDirection() const { return _currentDirection; }

    const int getFireballCount() const { return _thr_fireball_count; }

    const std::vector<std::shared_ptr<Entity>>& getFireballs() const { return _fireballs; }
    
    void initFireballs(Player* player);

    void latchFireballs();

    void threadLatchFireball(std::latch& latch, int index);

    void update(float dt);

    void signalShoot();
};    

} // namespace soul