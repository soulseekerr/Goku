
#include "fireball.h"
#include "logger.h"
#include "player.h"
#include "gamewindow.h"
#include "loaders.h"

using namespace soul;

Fireball::Fireball(
    FireballSystem& system,
    const std::string& tag, 
    float initialPosX, 
    float initialPosY, 
    float initialLifetime, 
    float speedX, 
    int direction, 
    float angle,
    bool active)
    : Animable(tag), _system(system) {
    _metrics.speedX = speedX;
    _metrics.lifetime = initialLifetime;
    _metrics.direction = direction;
    _initialPosition.x = initialPosX;
    _initialPosition.y = initialPosY;
    _angleSprite = angle;
    setActive(active);
}

void Fireball::reset(int index) {
    std::cout << std::format("Fireball::reset Index={}", index) << std::endl;

    const auto& player = _system.getPlayer();
    const soul::Vector2f& pos = player.getPosition();
    const auto& dir = player.getTransform().direction;
    
    _metrics.current_lifetime = _metrics.lifetime;
    _metrics.direction = dir;
    _initialPosition.x = pos.x - dir * (index - 1) * 100;
    _initialPosition.y = pos.y + 50.0f;
    float x = _initialPosition.x;
    float y = _initialPosition.y;
    _angleSprite = 90.0f * dir;

    this->setPosition(x, y);
    this->resetPosition();

    if (getPreviousDirection() != dir) {
        setPreviousDirection(dir);
        _angleSprite = _angleSprite + 180 * dir;
        this->setRotation(_angleSprite);
    }

    // std::cout << "Fireball::reset Position=" << std::format("x,y {:.1f} {:.1f} - {:.1f} {:.1f}", x, y, this->getPosition().x, this->getPosition().y) << std::endl;
    // std::cout << "Fireball::reset Angle=" << _angleSprite << std::endl;
    // std::cout << "Fireball::reset Direction=" << _metrics.direction << std::endl;

    setActive(true);
}

bool Fireball::update(float dt) {
    // Update bullet position based on velocity
    Animable::incrPositionX(_metrics.direction * _metrics.speedX * dt);
        // Animable::incrRotation(_metrics.direction * 10.0f * dt);
    
    // Decrease remaining lifetime
    _metrics.current_lifetime -= dt;

    // If lifetime expires, mark the bullet as dead
    if (_metrics.current_lifetime <= 0.001f) {
        std::cout << "Fireball is dead now, disabling it" << std::endl;
        setActive(false);
        _system.signalShoot();
        return isActive();
    }

    // std::cout << std::format(
    //     "Fireball::update position {:.1f},{:.1f} cur_lifetime {:.1f}",
    //     this->getPosition().x, this->getPosition().y, _metrics.current_lifetime) << std::endl;

     Animable::resetPosition();

    return Animable::update(dt);
}

void Fireball::render() {
    soul::GameWindow& gw = soul::GameWindow::getInstance();

    // std::cout << "Render "x << this->ID() << std::endl;

    // gw.window.draw(this->getSprite(), &_fireballShader.shader);

    if (isActive())
       gw.window.draw(this->getSprite());
}

void FireballSystem::initFireballs(Player* player, const std::string& filePath, float speedX, float lifetime) {
    _player = player;
    _fireballs.clear();
    _fireballs = VectorLoader<Fireball>::load(filePath, _thr_fireball_count, *player);
}

void FireballSystem::latchFireballs() {       
    // Create a latch with predefined number of threads to run in parallel
    // _thr_fireball_count: Number of Fireballs to create

// #ifdef DEBUG_MODE_STATES
logManager.log("{} fireballs to latch", _thr_fireball_count);
// #endif

    std::latch latch(_thr_fireball_count);

    std::vector<std::jthread> jthreads;
    for (int i = 0; i < _thr_fireball_count; ++i) {
        jthreads.emplace_back(
            [this, &latch, i]() { 
                // Entity is active now
                // Update data in the entity before rendering it
                _fireballs[i]->reset(i);

            latch.count_down();  // Decrease latch counter
        });
    }

    latch.wait();  // Wait until all threads call count_down()
}

void FireballSystem::signalShoot() {

    _thr_current_count_fireball.increment();

    if (_thr_current_count_fireball.get() == _thr_fireball_count) {

#ifdef DEBUG_MODE_STATES
        logManager.logDebug("Signal shoot reached {} fireballs. Re-enable shoot.", _thr_fireball_count);
#endif

        _thr_current_count_fireball.set(0);
        
        if (_player) {
            _player->input.shoot = false;
            _player->input.is_shooting = false;
        }
    }
}

void FireballSystem::update(float dt) {
    // Handle Inputs in the Animable objects
    for (const auto& e : _fireballs) {
        // Update the states and animation associated to the current movement or action
        if (e->isActive()) e->update(dt);
    }
}