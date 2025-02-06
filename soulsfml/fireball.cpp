
#include "fireball.h"
#include "core/logger.h"
#include "player.h"

using namespace soul;

Fireball::Fireball(
    Player& player,
    const std::string& tag, 
    int initialPosX, 
    int initialPosY, 
    float initialLifetime, 
    float speedX, 
    int direction, 
    float angle,
    bool active)
    : Animable(tag), _playerRef(player) {
    _metrics.speedX = speedX;
    _metrics.lifetime = initialLifetime;
    _metrics.direction = direction;
    _initialPosition.x = initialPosX;
    _initialPosition.y = initialPosY;
    _angleSprite = angle;
    setActive(active);
}

void Fireball::loadData() {
    // auto& logManager = LoggerManager::getInstance();

    const std::string filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/fireballs.png";
    
    // Player position adjusted to shoot more or less from the hand
    auto posX = _initialPosition.x;
    auto posY = _initialPosition.y;

    const soul::sSpriteData spriteData = {
        filePath, // Filename for the image file
        soul::Vector2f(1.0f, 1.0f), // Scaling the sprite from the image
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

    Animable::resetPosition();
}

bool Fireball::update(float dt) {

    // Update bullet position based on velocity
    Animable::incrPositionX(_metrics.direction * _metrics.speedX * dt);

    // Decrease remaining lifetime
    _metrics.lifetime -= dt;

    // If lifetime expires, mark the bullet as dead
    if (_metrics.lifetime <= 0) {
        setActive(false);
        _playerRef.input.shoot = false;
        _playerRef.input.is_shooting = false;
    }

    std::cout << std::format("lifetime = %.1f", _metrics.lifetime) << std::endl;

     Animable::resetPosition();

    return Animable::update(dt);
}
