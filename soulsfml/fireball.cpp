
#include "fireball.h"
#include "core/logger.h"
#include "player.h"

using namespace soul;

Fireball::Fireball(
    FireballSystem& system,
    const std::string& tag, 
    int initialPosX, 
    int initialPosY, 
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

void Fireball::loadData() {
    const std::string filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/fireballs.png";
    
    // Player position adjusted to shoot more or less from the hand
    auto posX = _initialPosition.x;
    auto posY = _initialPosition.y;

    const soul::sSpriteData spriteData = {
        filePath, // Filename for the image file
        soul::Vector2f(1.5f, 1.5f), // Scaling the sprite from the image
        soul::Vector2i(10, 128), // Initial px position in the Sprite sheet
        soul::Vector2i(70, 70), // Initial px size in the Sprite sheet
        soul::Vector2i(posX, posY), // Coordinates px of screen as they are defined 0,0 from top left.
        false, // Filter Transparency By Color
        soul::Color(0, 0, 0, 0) // Filtering Color
    };

    const soul::sTransformScalars scalars = {
        _metrics.speedX, // initialVelocityX
        0.0f,  // initialVelocityY
        0.0f, // gravity
        _metrics.direction, // direction
        _angleSprite, // angle
        450 // groundY
    };

    Animable::load(spriteData, scalars);

    auto animationFire = std::make_shared<SpriteAnimation>();

    auto duration = 0.05f;
    animationFire->addFrame(0, 0, 70, 70, duration);
    animationFire->addFrame(140, 0, 70, 70, duration);
    animationFire->addFrame(210, 0, 70, 70, duration);
    animationFire->addFrame(280, 0, 70, 70, duration);
    animationFire->addFrame(350, 0, 70, 70, duration);
    animationFire->addFrame(420, 0, 70, 70, duration);
    animationFire->addFrame(490, 0, 70, 70, duration);
    animationFire->addFrame(560, 0, 70, 70, duration);
    animationFire->addFrame(630, 0, 70, 70, duration);
    animationFire->addFrame(700, 0, 70, 70, duration);
    animationFire->addFrame(770, 0, 70, 70, duration);
    animationFire->addFrame(840, 0, 70, 70, duration);
    animationFire->addFrame(910, 0, 70, 70, duration);
    animationFire->addFrame(980, 0, 70, 70, duration);
    animationFire->addFrame(1050, 0, 70, 70, duration);
    Animable::addAnimation(soul::AnimationState::ShootFireball, animationFire);
}

void Fireball::updateData(int index) {
    // auto& sp = this->getSprite();

    const auto& playerPos = _system.getPlayer().getPosition();
    const int& dir = _system.getPlayer().getTransform().direction;

    int initialPosX = playerPos.x + dir * (index-1) * 70;
    float speedX = 100.0f;

    _metrics.speedX = speedX;
    _metrics.lifetime = 1.0f;
    _metrics.direction = dir;
    _initialPosition.x = initialPosX;
    _initialPosition.y = playerPos.y;
    _angleSprite = -90.0f * dir;

    // std::cout << "updateData PosX: " << _initialPosition.x << "   PosY: " << _initialPosition.y << std::endl;

    // sp.setPosition(sf::Vector2f(_initialPosition.x + 0.01f, _initialPosition.y + 0.01f));
    // sp.setScale(sp.getScale() * (dir * 1.0f));

    // std::cout << "Animable PosX: " << sp.getPosition().x << std::endl;

    Animable::setVelocityX(speedX);

    setActive(true);
}

bool Fireball::update(float dt) {
    // Update bullet position based on velocity
    Animable::incrPositionX(_metrics.direction * _metrics.speedX * dt);
    Animable::incrRotation(_metrics.direction * 10.0f * dt);

    // Decrease remaining lifetime
    _metrics.lifetime -= dt;

    // If lifetime expires, mark the bullet as dead
    if (_metrics.lifetime <= 0) {
        std::cout << "Fireball is dead now, disabling it" << std::endl;
        setActive(false);
        _system.signalShoot();
        return false;
    }

    // std::cout << std::format("pos x: {:.0f} pos y: {:.0f} lifetime = {:.1f}", x, y, _metrics.lifetime) << std::endl;

     Animable::resetPosition();

    return Animable::update(dt);
}

void FireballSystem::initFireballs(Player* player) {
    _player = player;
    auto& transform = _player->getTransform();
    auto& pos = _player->getPosition();

    // Allocate fireballs ready to use
    for (auto id = 1; id <= _thr_fireball_count; id++) {

        logManager.log("Create Fireball {}", id);

        int direction = transform.direction;
        int initialPosX = pos.x + direction * (id-1) * 70;
        int initialPosY = pos.y;
        float initialLifetime = 2.0f;
        float speedX = 300.0f;
        float angle = -90.0f * direction;  

        // Dont use auto keyword here to allow implicit casting
        std::shared_ptr<soul::Entity> fb;

        fb = std::make_shared<soul::Fireball>(
            *this,
            "Fireball",
            initialPosX,
            initialPosY,
            initialLifetime,
            speedX,
            direction,
            angle);

        fb->loadData();

        logManager.log("Add inactive Fireball {} to entities", id);

        // Fireball is inactive until we shoot
        fb->setActive(false);

        // Add it in the Pool of instances ready to use
        _fireballs.push_back(fb);
    }
}

void FireballSystem::latchFireballs() {       
    // Create a latch with predefined number of threads to run in parallel
    // _thr_fireball_count: Number of Fireballs to create
    auto count = getFireballCount();

#ifdef DEBUG_MODE_STATES
    logManager.logDebug("{} fireballs to latch", count);
#endif

    std::latch latch(count);

    std::vector<std::jthread> jthreads;
    for (int i = 0; i < count; ++i) {
        jthreads.emplace_back([this, &latch, i]() { threadLatchFireball(latch, i); });
    }

    latch.wait();  // Wait until all threads call count_down()
}

void FireballSystem::threadLatchFireball(std::latch& latch, int index) {
    auto& logManager = soul::LoggerManager::getInstance();
    
    // Get Next Active Fireball
    auto fb = _fireballs[index];

    if (fb) {
        // Entity is active now
        // Update data in the entity before rendering it
        fb->updateData(index);

    } else{
        logManager.logWarn("No Fireball available from the pool!");
    }

    latch.count_down();  // Decrease latch counter
}

void FireballSystem::signalShoot() {

    _thr_current_count_fireball++;

    if (_thr_current_count_fireball == _thr_fireball_count) {

#ifdef DEBUG_MODE_STATES
        logManager.logDebug("Signal shoot reached {} fireballs. Re-enable shoot.", _thr_fireball_count);
#endif

        _thr_current_count_fireball = 0;
        
        if (_player) {
            _player->input.shoot = false;
            _player->input.is_shooting = false;
        }
    }
}

std::shared_ptr<Entity>& FireballSystem::getPoolable(const int index) {
    // std::scoped_lock lock(_mutex_poolable); // it is done in a thread
    // auto it = std::find_if(_poolables.begin(), _poolables.end(),
        // [](std::shared_ptr<Entity>& e) {
            // return !e->isActive();  // get non active entity
        // });

    // If a valid entity is found, return it; otherwise, return optional nullopt
    // return (it != _poolables.end()) ? std::make_optional(*it) : std::nullopt;
    // if (it != _poolables.end()) {
    //     (*it)->setActive(true);  // Ensure the entity is marked active
    //     return std::make_optional(*it);
    // }

    // return std::nullopt;
    return _fireballs[index];
}

void FireballSystem::update(float dt) {
    // Handle Inputs in the Animable objects
    for (const auto& e : _fireballs) {
        // Update the states and animation associated to the current movement or action
        if (e->isActive()) e->update(dt);
    }
}