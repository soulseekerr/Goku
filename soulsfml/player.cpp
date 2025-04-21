
#include "player.h"
#include "state.h"

#include <format>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace soul;
using json = nlohmann::json;

Player::Player(const std::string& name) : Animable(name) {}

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
        // if (e->isActive()) 
        e->render();
    }
}

void Player::initFireballs(const std::string& filePath, float speedX, float lifetime) {
    _fireballs.initFireballs(this, filePath, speedX, lifetime);
}