
#include "player.h"
#include "state.h"
#include "fireball.h"

#include <format>

using namespace soul;

Player::Player(const std::string& name) : Animable(name) {}

// Load the sprite data and the transform scalars
void Player::loadData() {
    
    const std::string filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/Kid Goku.png";

    const soul::sSpriteData spriteData = {
        filePath, // Filename for the image file
        soul::Vector2f(1.2f, 1.2f), // Scaling the sprite from the image
        soul::Vector2i(10, 128), // Initial px position in the Sprite sheet
        soul::Vector2i(80, 102), // Initial px size in the Sprite sheet
        soul::Vector2i(300, 450), // Coordinates px of screen as they are defined 0,0 from top left.
        false, // Filter Transparency By Color
        soul::Color(0, 0, 0, 0) // Filtering Color
    };

    const soul::sTransformScalars scalars = {
        6.0f, // initialVelocityX
        -700.0f,  // initialVelocityY
        1700.0f, // gravity
        1, // direction
        0.0f, // angle
        450 // groundY
    };
    
    Animable::load(spriteData, scalars);

    // Idle 2nd line
    auto animIdle = std::make_shared<soul::SpriteAnimation>();
    animIdle->addFrame(10, 128, 80, 102, 0.25f);
    animIdle->addFrame(100, 128, 80, 102, 0.25f);
    animIdle->addFrame(190, 128, 80, 102, 0.25f);
    animIdle->addFrame(100, 128, 80, 102, 0.25f);
    Animable::addAnimation(soul::AnimationState::Idle, animIdle);

    // Walk
    auto animWalk = std::make_shared<soul::SpriteAnimation>();
    animWalk->addFrame(645, 235, 110, 90, 0.05f);
    Animable::addAnimation(soul::AnimationState::Walk, animWalk);

    // Jump 6th line
    auto animJump = std::make_shared<soul::SpriteAnimation>();
    animJump->addFrame(190, 550, 80, 115, 0.10f);
    animJump->addFrame(190, 550, 80, 115, 0.10f);
    animJump->addFrame(190, 550, 80, 115, 0.10f);
    animJump->addFrame(190, 550, 80, 115, 0.10f);
    animJump->addFrame(460, 550, 80, 115, 0.20f);
    animJump->addFrame(460, 550, 80, 115, 0.10f);
    animJump->addFrame(10, 128, 80, 102, 0.20f);
    Animable::addAnimation(soul::AnimationState::Jump, animJump);

    auto animPunch = std::make_shared<soul::SpriteAnimation>();
    animPunch->addFrame(420, 1535, 90, 85, 0.10f);
    animPunch->addFrame(510, 1535, 70, 85, 0.10f);
    animPunch->addFrame(580, 1535, 85, 85, 0.10f);
    Animable::addAnimation(soul::AnimationState::Punch, animPunch);

    auto animShoot = std::make_shared<SpriteAnimation>();
    animShoot->addFrame(10, 2470, 70, 88, 0.1f);
    animShoot->addFrame(85, 2470, 80, 88, 0.1f);
    animShoot->addFrame(175, 2470, 80, 88, 0.1f);
    animShoot->addFrame(255, 2470, 80, 88, 0.1f);
    animShoot->addFrame(335, 2470, 80, 88, 0.1f);
    animShoot->addFrame(415, 2470, 80, 88, 0.1f);
    animShoot->addFrame(495, 2470, 80, 88, 0.1f);
    animShoot->addFrame(582, 2470, 80, 88, 0.1f);
    animShoot->addFrame(655, 2470, 80, 88, 0.1f);
    animShoot->addFrame(655, 2470, 80, 88, 0.1f);
    animShoot->addFrame(335, 2470, 80, 88, 0.1f);
    animShoot->addFrame(10, 2470, 70, 88, 0.1f);
    Animable::addAnimation(soul::AnimationState::Shoot, animShoot);

    Animable::resetPosition();

    soul::IdleState* idleState = Animable::registerMovement<soul::IdleState>(soul::AnimationState::Idle);
    soul::JumpState* jumpState = Animable::registerMovement<soul::JumpState>(soul::AnimationState::Jump);
    soul::WalkState* walkState = Animable::registerMovement<soul::WalkState>(soul::AnimationState::Walk);
    soul::ActionIdleState* actionIdleState = Animable::registerAction<soul::ActionIdleState>(soul::AnimationState::ActionIdle);
    soul::PunchState* punchState = Animable::registerAction<soul::PunchState>(soul::AnimationState::Punch);
    soul::ShootState* shootState = Animable::registerAction<soul::ShootState>(soul::AnimationState::ShootFireball);

    idleState->defineDependencies(stateManager);
    jumpState->defineDependencies(stateManager);
    walkState->defineDependencies(stateManager);
    actionIdleState->defineDependencies(stateManager);
    punchState->defineDependencies(stateManager);
    shootState->defineDependencies(stateManager);

    Animable::setAnimationState(soul::AnimationState::Idle);
    Animable::setMovementState(*Animable::getMovementState<soul::IdleState>(soul::AnimationState::Idle));
    Animable::setActionState(*Animable::getActionState<soul::ActionIdleState>(soul::AnimationState::ActionIdle));
}

bool Player::update(float dt) {
    auto isActive =  Animable::update(dt);

    // Shoot Fireball
    // Check if the player is shooting, if yes do not allow shooting
    if (input.shoot && !input.is_shooting) {
        input.is_shooting = true; // we lock the shoot until fireball is dead
        input.shoot = false;  // applied once in case we keep pressing the button 
        spawnFireball();
    }
    
    return isActive;
}

void Player::spawnFireball() {
    // Logger access
    auto& logManager = soul::LoggerManager::getInstance();
    // Container for entities
    soul::EntityManager&  entities = soul::EntityManager::getInstance();

    const std::string filePath = "/Users/soulseeker/Projects/GitHub/gokugame/debug/textures/fireballs.png";

    int initialPosX = getPosition().x;
    int initialPosY = getPosition().y;
    float initialLifetime = 1.0f;
    float speedX = 250.0f;
    int direction = getTransform().direction;
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

    auto animFire = std::make_shared<soul::SpriteAnimation>();
    animFire->addFrame(0, 0, 70, 70, 0.05f);
    animFire->addFrame(140, 0, 70, 70, 0.05f);
    animFire->addFrame(210, 0, 70, 70, 0.05f);
    animFire->addFrame(280, 0, 70, 70, 0.05f);
    animFire->addFrame(350, 0, 70, 70, 0.05f);
    animFire->addFrame(420, 0, 70, 70, 0.05f);
    animFire->addFrame(490, 0, 70, 70, 0.05f);
    animFire->addFrame(560, 0, 70, 70, 0.05f);
    animFire->addFrame(630, 0, 70, 70, 0.05f);
    animFire->addFrame(700, 0, 70, 70, 0.05f);
    animFire->addFrame(770, 0, 70, 70, 0.05f);
    animFire->addFrame(840, 0, 70, 70, 0.05f);
    animFire->addFrame(910, 0, 70, 70, 0.05f);
    animFire->addFrame(980, 0, 70, 70, 0.05f);
    animFire->addFrame(1050, 0, 70, 70, 0.05f);
    Animable::addAnimation(soul::AnimationState::ShootFireball, animFire);

    Animable::resetPosition();

    entities.addEntity(fb);

    logManager.log("New Fireball ID {})", fb->ID());
}