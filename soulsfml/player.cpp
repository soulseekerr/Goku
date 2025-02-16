
#include "player.h"
#include "state.h"

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
    animPunch->addFrame(420, 1535, 90, 84, 0.20f);
    animPunch->addFrame(510, 1535, 70, 84, 0.15f);
    animPunch->addFrame(580, 1535, 85, 84, 0.15f);
    Animable::addAnimation(soul::AnimationState::Punch, animPunch);

    auto animShoot = std::make_shared<SpriteAnimation>();
    animShoot->addFrame(10, 2470, 70, 88, 0.1f);
    animShoot->addFrame(85, 2470, 80, 88, 0.1f);
    animShoot->addFrame(175, 2470, 80, 88, 0.1f);
    animShoot->addFrame(255, 2470, 80, 88, 0.1f);
    animShoot->addFrame(335, 2470, 80, 88, 0.1f);
    animShoot->addFrame(415, 2470, 80, 88, 0.1f);
    animShoot->addFrame(495, 2470, 80, 88, 0.1f);
    Animable::addAnimation(soul::AnimationState::Shoot, animShoot);

    auto animKick = std::make_shared<SpriteAnimation>();
    animKick->addFrame(270, 1640, 85, 87, 0.15f);
    animKick->addFrame(360, 1640, 85, 87, 0.15f);
    animKick->addFrame(450, 1640, 95, 87, 0.15f);
    animKick->addFrame(360, 1640, 85, 87, 0.15f);
    Animable::addAnimation(soul::AnimationState::Kick, animKick);

    auto animKick2 = std::make_shared<SpriteAnimation>();
    animKick2->addFrame(560, 1640, 70, 88, 0.15f);
    animKick2->addFrame(630, 1640, 70, 88, 0.15f);
    animKick2->addFrame(700, 1640, 80, 88, 0.15f);
    animKick2->addFrame(630, 1640, 70, 88, 0.15f);
    Animable::addAnimation(soul::AnimationState::Kick2, animKick2);

    auto animJumpKick = std::make_shared<SpriteAnimation>();
    animJumpKick->addFrame(810, 1640, 80, 88, 0.15f);
    animJumpKick->addFrame(890, 1640, 60, 88, 0.15f);
    animJumpKick->addFrame(1030, 1640, 65, 88, 0.15f);
    animJumpKick->addFrame(1095, 1640, 88, 88, 0.15f);
    animJumpKick->addFrame(1185, 1640, 88, 88, 0.15f);
    Animable::addAnimation(soul::AnimationState::JumpKick, animJumpKick);

    auto animDef = std::make_shared<SpriteAnimation>();
    animDef->addFrame(10, 1535, 85, 84, 0.10f);
    animDef->addFrame(90, 1535, 85, 84, 0.10f);
    animDef->addFrame(170, 1535, 85, 84, 0.10f);
    animDef->addFrame(250, 1535, 85, 84, 0.10f);
    animDef->addFrame(170, 1535, 85, 84, 0.10f);
    animDef->addFrame(90, 1535, 85, 84, 0.10f);
    animDef->addFrame(10, 1535, 85, 84, 0.10f);
    Animable::addAnimation(soul::AnimationState::Defensive, animDef);

    auto animPunchStick = std::make_shared<SpriteAnimation>();
    animPunchStick->addFrame(10, 1875, 75, 90, 0.10f);
    animPunchStick->addFrame(80, 1865, 65, 102, 0.10f);
    animPunchStick->addFrame(140, 1855, 70, 112, 0.10f);
    animPunchStick->addFrame(205, 1855, 95, 112, 0.10f);
    animPunchStick->addFrame(205, 1855, 95, 112, 0.10f);
    animPunchStick->addFrame(10, 1875, 75, 90, 0.10f);
    Animable::addAnimation(soul::AnimationState::PunchStick, animPunchStick);

    // Knocked sprite 10th line
    auto animKnockedOut = std::make_shared<SpriteAnimation>();
    animKnockedOut->addFrame(10, 1020, 80, 90, 0.40f);
    animKnockedOut->addFrame(90, 1020, 110, 90, 0.40f);
    animKnockedOut->addFrame(200, 1020, 90, 90, 0.40f);
    animKnockedOut->addFrame(300, 1020, 110, 90, 0.40f);
    animKnockedOut->addFrame(400, 1020, 110, 91, 0.75f);
    animKnockedOut->addFrame(400, 1020, 110, 91, 0.75f);
    animKnockedOut->addFrame(400, 1020, 110, 91, 0.75f);
    animKnockedOut->addFrame(90, 1020, 110, 90, 0.30f);
    animKnockedOut->addFrame(10, 1020, 80, 90, 0.30f);
    Animable::addAnimation(soul::AnimationState::Knocked, animKnockedOut);

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

    soul::IdleState* idleState = Animable::registerMovement<soul::IdleState>(soul::AnimationState::Idle);
    soul::JumpState* jumpState = Animable::registerMovement<soul::JumpState>(soul::AnimationState::Jump);
    soul::WalkState* walkState = Animable::registerMovement<soul::WalkState>(soul::AnimationState::Walk);
    soul::ActionIdleState* actionIdleState = Animable::registerAction<soul::ActionIdleState>(soul::AnimationState::ActionIdle);
    soul::PunchState* punchState = Animable::registerAction<soul::PunchState>(soul::AnimationState::Punch);
    soul::PunchStickState* punchStickState = Animable::registerAction<soul::PunchStickState>(soul::AnimationState::PunchStick);
    soul::ShootState* shootState = Animable::registerAction<soul::ShootState>(soul::AnimationState::ShootFireball);
    soul::KickState* kickState = Animable::registerAction<soul::KickState>(soul::AnimationState::Kick);
    soul::Kick2State* kick2State = Animable::registerAction<soul::Kick2State>(soul::AnimationState::Kick2);
    soul::DefensiveState* defState = Animable::registerAction<soul::DefensiveState>(soul::AnimationState::Defensive);
    soul::KnockedOutState* koState = Animable::registerAction<soul::KnockedOutState>(soul::AnimationState::Knocked);

    idleState->defineDependencies(stateManager);
    jumpState->defineDependencies(stateManager);
    walkState->defineDependencies(stateManager);
    actionIdleState->defineDependencies(stateManager);
    punchState->defineDependencies(stateManager);
    punchStickState->defineDependencies(stateManager);
    shootState->defineDependencies(stateManager);
    kickState->defineDependencies(stateManager);
    kick2State->defineDependencies(stateManager);
    defState->defineDependencies(stateManager);
    koState->defineDependencies(stateManager);

    Animable::setAnimationState(soul::AnimationState::Idle);
    Animable::setMovementState(*Animable::getMovementState<soul::IdleState>(soul::AnimationState::Idle));
    Animable::setActionState(*Animable::getActionState<soul::ActionIdleState>(soul::AnimationState::ActionIdle));

    // provide Player position and other metrics
    _fireballs.initFireballs(this);
}

bool Player::update(float dt) {
    // Update the animation of the player
    auto isActive =  Animable::update(dt);

    // Shoot Fireball
    // Check if the player is shooting, if yes do not allow shooting
    if (input.shoot && !input.is_shooting) {
    
#ifdef DEBUG_MODE_STATES
        logManager.logDebug("SHOOT.");
#endif

        input.is_shooting = true; // we lock the shoot until fireball is dead
        input.shoot = false;  // applied once in case we keep pressing the button 

        _fireballs.latchFireballs();
    }

    // Update the fireballs
    _fireballs.update(dt);

    // Return current status of the entity
    return isActive;
}

void Player::render() {

    Animable::render();

    for (const auto& e : _fireballs.getFireballs()) {
        if (e->isActive()) e->render();
    }
}