
#include "player.h"
#include "state.h"

#include <format>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace soul;
using json = nlohmann::json;

Player::Player(const std::string& name) : Animable(name) {}

// Load the sprite data and the transform scalars
void Player::loadData() {
    std::ifstream file("/Users/soulseeker/Projects/GitHub/Goku20250301/Goku/data/player.json");
    json data;
    file >> data;

    _attributes.currentHealth = data["attributes"]["current_health"];
    _attributes.maxHealth = data["attributes"]["max_health"];
    _attributes.currentMana = data["attributes"]["current_mana"];
    _attributes.maxMana = data["attributes"]["max_mana"];
    _attributes.currentExp = data["attributes"]["current_exp"];
    _attributes.maxExp = data["attributes"]["max_exp"];
    _attributes.level = data["attributes"]["level"];

    const soul::sSpriteData spriteData = {
        // Filename for the image file
        data["spriteData"]["filePath"],
        // Scaling the sprite from the image
        soul::Vector2f(data["spriteData"]["scale"][0], data["spriteData"]["scale"][1]),
        // Initial px position in the Sprite sheet
        soul::Vector2i(data["spriteData"]["spriteSheetPosition"][0], data["spriteData"]["spriteSheetPosition"][1]),
        // Initial px size in the Sprite sheet
        soul::Vector2i(data["spriteData"]["spriteSheetSize"][0], data["spriteData"]["spriteSheetSize"][1]),
        // Coordinates px of screen as they are defined 0,0 from top left.
        soul::Vector2f(data["spriteData"]["screenPosition"][0], data["spriteData"]["screenPosition"][1]), 
        // Filter Transparency By Color
        data["spriteData"]["filterTransparency"], 
        // Filtering Color
        soul::Color(
            data["spriteData"]["filterColor"][0],
            data["spriteData"]["filterColor"][1],
            data["spriteData"]["filterColor"][2],
            data["spriteData"]["filterColor"][3]) 
    };

    const soul::sTransformScalars scalars = {
        data["transformScalars"]["initialVelocityX"],
        data["transformScalars"]["initialVelocityY"],
        data["transformScalars"]["gravity"],
        data["transformScalars"]["direction"],
        data["transformScalars"]["angle"],
        data["transformScalars"]["groundY"]
    };

    Animable::load(spriteData, scalars);

    std::map<std::string, soul::AnimationState> anim_states = {
        {"Idle", soul::AnimationState::Idle},
        {"Walk", soul::AnimationState::Walk},
        {"Jump", soul::AnimationState::Jump},
        {"Punch", soul::AnimationState::Punch},
        {"PunchStick", soul::AnimationState::PunchStick},
        {"Shoot", soul::AnimationState::Shoot},
        {"Kick", soul::AnimationState::Kick},
        {"Kick2", soul::AnimationState::Kick2},
        {"JumpKick", soul::AnimationState::JumpKick},
        {"Defensive", soul::AnimationState::Defensive},
        {"KnockedOut", soul::AnimationState::Knocked},
        {"ShootFireball", soul::AnimationState::ShootFireball}
    };

    for (const auto& [anim_name, anim_state] : anim_states) {
        auto anim = data["animations"][anim_name];

        if (anim.empty()) {
            logManager.logError("Animation not found: " + anim_name);
            continue;
        }
    
        auto spriteAnim = std::make_shared<soul::SpriteAnimation>();
        for (const auto& frame : anim) {
            spriteAnim->addFrame(frame["x"], frame["y"], frame["width"], frame["height"], frame["duration"]);
        }

        Animable::addAnimation(anim_state, spriteAnim);
    }

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
    _fireballs.initFireballs(this, 
        data["fireballs"]["filePath"].get<std::string>(),
        data["fireballs"]["speedX"].get<float>(),
        data["fireballs"]["lifetime"].get<float>());
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