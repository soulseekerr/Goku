#pragma once

#include "animable.h"
#include "spriteanimation.h"
#include "pathmanager.h"
#include "shader.h"

#include <latch>

namespace soul {

class Player;

struct sFireballMetrics {
    float   speedX {250.0f};
    float   lifetime {1.0f};
    float   current_lifetime {1.0f};
    int     direction {1};
};

class FireballSystem;

/**
 * @brief Fireball class
 * Class that represents a fireball entity
 */
class Fireball : public Animable {
private:
    // Path Manager
    PathManager& pathManager = PathManager::getInstance();
    // Reference to the system of fireballs
    FireballSystem& _system;
    sFireballMetrics _metrics;
    soul::Vector2f _initialPosition;
    float _angleSprite;
    int _previousDirection {1};

    // FireballShader _fireballShader;

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

    // Update the entity with specialized logic
    virtual bool update(float dt) override;

    virtual void render() override;

    void reset(int id);

    const float& getSpeedX() const { return _metrics.speedX; }
    const float& getLifetime() const { return _metrics.lifetime; }

    int getPreviousDirection() const { return _previousDirection; }
    void setPreviousDirection(int dir) { _previousDirection = dir; }
};

/**
 * @brief FireballSystem class
 * 
 */
class FireballSystem {
private:
    soul::LoggerManager& logManager = soul::LoggerManager::getInstance();
    // Path Manager
    PathManager& pathManager = PathManager::getInstance();
    // Pointer to the instance of Player
    Player* _player;    
    // Number of fireballs to shoot
    static constexpr int _thr_fireball_count = 2;
    // Container for poolables (flyweight)
    std::vector<std::shared_ptr<Fireball>> _fireballs;
    // Atomic counter variable used as lock for fireball shots
    // std::atomic<int> _thr_current_count_fireball{0};
    SafeNumeric<int> _thr_current_count_fireball{0};
        
public:
    FireballSystem() : _player(nullptr) {}
    ~FireballSystem() = default;

    const Player& getPlayer() const { return *_player; } 

    int getFireballCount() const { return _thr_fireball_count; }

    const std::vector<std::shared_ptr<Fireball>>& getFireballs() const { return _fireballs; }
    
    void initFireballs(Player* player, const std::string& filePath, float speedX, float lifetime);

    void latchFireballs();

    void update(float dt);

    void signalShoot();
};    

} // namespace soul