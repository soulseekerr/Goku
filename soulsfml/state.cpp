
#include "state.h"
#include "animable.h"

using namespace soul;

/**
 * @brief Get Animation State from String
 */
soul::AnimationState animationStateFromString(const std::string& str) {
    static const std::unordered_map<std::string, soul::AnimationState> s_map = {
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
        {"Knocked", soul::AnimationState::Knocked},
        {"ShootFireball", soul::AnimationState::ShootFireball}
    };

    auto it = s_map.find(str);
    if (it != s_map.end()) return it->second;
    throw std::runtime_error("Unknown AnimationState: " + str);
}

void MovementState::translateX(Animable& a, float dt) {

    if (a.input.left) {
        a._sprite->transform.direction = -1;
        a._sprite->velocity.x = -a._sprite->transform.initialVelocityX; // Move left
    }
    else if (a.input.right) {
        a._sprite->transform.direction = 1;
        a._sprite->velocity.x = a._sprite->transform.initialVelocityX; // Move right
        a._sprite->setScaleFactor(a._sprite->scale.x, a._sprite->scale.y);
    }

    // Update player position based on velocity
    // logManager.logDebug("MovementState::translateX: incrPositionX by dt {} speed {} velocity {} pos {} {}",  
        // dt, a._sprite->transform.moveSpeed, a._sprite->velocity.x, a._sprite->position.x, a._sprite->position.y);

    a.incrPositionX(a._sprite->velocity.x);

    // Set Sprite factor if we are facing left or right
    a._sprite->setScaleFactor(a._sprite->transform.direction * a._sprite->scale.x, a._sprite->scale.y);
}

void MovementState::jumpY(Animable& a, float dt) {
    
    if (!a.input.is_jumping) {
        // Define current velocity for jump
        a._sprite->velocity.y = a._sprite->transform.initialVelocityY; 

        a.input.is_jumping = true;
        a.input.is_on_ground = false;
    }

    // Update player position based on velocity
    a._sprite->position.y += a._sprite->velocity.y * dt;

}

bool MovementState::landY(Animable& a, float dt) {

    if (a.input.is_jumping) {
        // If we are in jump mode we apply gravity in positive terms
        // because screen coords we go down in positive
        // logManager.logDebug("MovementState::landY by dt {} speed {} velocity {} posy {}", 
        //     dt, a._sprite->transform.initialVelocityX, a._sprite->velocity.x, a._sprite->position.y);

        a._sprite->velocity.y += a._sprite->transform.gravity * dt; // Apply gravity
    }

    // we are on a platform 
    if (a.input.is_on_ground) {
        a._sprite->velocity.y = 0;
        a.input.is_jumping = false;
        return true;
    }

    // Update player position based on velocity
    a._sprite->position.y += a._sprite->velocity.y * dt;

    // We are in jump mode and we are back on the ground, applying gravity
    // if (a.getWindowPositionY() - a._sprite->position.y >= 600) {
    if (a._sprite->position.y >= a.getGroundY()) {
        a._sprite->position.y = a.getGroundY();
        a._sprite->velocity.y = 0;
        a.input.is_jumping = false;
        a.input.is_on_ground = true;
        return true;
    }

    return false;
}

void IdleState::defineDependencies(StateManager& stateMgr) {
    _walkStateRef = stateMgr.getMovementState<WalkState>(AnimationState::Walk);
    _jumpStateRef = stateMgr.getMovementState<JumpState>(AnimationState::Jump);
}

void IdleState::handleInput(Animable& a) {
    // Transition to Walk, Jump, or Punch based on input
    if (a.input.left || a.input.right) {
        
        a.setMovementState(*_walkStateRef);
    }
    
    if (a.input.up) {
        
        a.setMovementState(*_jumpStateRef);

    } else {
        
        a.setVelocityX(0.0f); // Stop horizontal movement
    }
}

void IdleState::update(Animable& a, float dt) {
    // logManager.logDebug("IdleState::update");
    // No special update logic for Idle (the animation will auto-play)
}

void IdleState::enter(Animable& a) {
    // logManager.logDebug("IdleState::enter");
    a.setAnimationState(AnimationState::Idle); // Set idle animation
}

void JumpState::defineDependencies(StateManager& stateMgr) {
    _walkStateRef = stateMgr.getMovementState<WalkState>(AnimationState::Walk);
    _idleStateRef = stateMgr.getMovementState<IdleState>(AnimationState::Idle);
}

void JumpState::handleInput(Animable& a) {
    // During the jump, inputs might be ignored or limited
    // Cannot combine with other Movement state 
}

void JumpState::update(Animable& a, float dt) {
    // logManager.logDebug("JumpState::update");

    // Update jump logic (e.g., gravity, position updates)
    // After landing, transition back to Idle or Walk
    
    if (a.input.left) {
        translateX(a, dt);
    }

    if (a.input.right) {
        translateX(a, dt);
    }

    // we just start the jump
    if (!a.input.is_jumping) {
        jumpY(a, dt);
        a.resetPosition();
        return;
    }
    
    // Player is jumping here and has to land on the ground
    if (landY(a, dt)) {
        a.setMovementState(*_idleStateRef);
    }

    a.resetPosition();
}

void JumpState::enter(Animable& a) {
    // logManager.logDebug("JumpState::enter");
    a.setAnimationState(AnimationState::Jump); // Set jump animation
}

void WalkState::defineDependencies(StateManager& stateMgr) {
    _jumpStateRef = stateMgr.getMovementState<JumpState>(AnimationState::Jump);
    _idleStateRef = stateMgr.getMovementState<IdleState>(AnimationState::Idle);
}

void WalkState::handleInput(Animable& a) {
    // if not moving, we go to Idle
    if (!a.input.left && !a.input.right) {
        a.setMovementState(*_idleStateRef);
    }
    
    // if we jump while walking, we switch the state to Jump
    if (a.input.up) {
        a.setMovementState(*_jumpStateRef);
    }
}

void WalkState::update(Animable& a, float dt) {
    // Update walk logic (e.g., moving the player position)
    translateX(a, dt);
    a.resetPosition();
}

void WalkState::enter(Animable& a) {
    // logManager.logDebug("WalkState::enter");
    a.setAnimationState(AnimationState::Walk); // Set walk animation
}

void ActionState::defineDependencies(StateManager& stateMgr) {
    _idleStateRef = stateMgr.getActionState<ActionIdleState>(AnimationState::ActionIdle);
    _idleMovementStateRef = stateMgr.getMovementState<IdleState>(AnimationState::Idle);
}

bool ActionState::playOnce(Animable& a) {
    auto& currentAnim = a.getAnimations()->getCurrentAnimation();

    if (!currentAnim->isLastFrame()) 
        return false;

    return true;
}

void ActionIdleState::defineDependencies(StateManager& stateMgr) {
    ActionState::defineDependencies(stateMgr);
    _punchStateRef = stateMgr.getActionState<PunchState>(AnimationState::Punch);
    _punchStickStateRef = stateMgr.getActionState<PunchStickState>(AnimationState::PunchStick);
    _shootStateRef = stateMgr.getActionState<ShootState>(AnimationState::ShootFireball);
    _kickStateRef = stateMgr.getActionState<KickState>(AnimationState::Kick);
    _kick2StateRef = stateMgr.getActionState<Kick2State>(AnimationState::Kick2);
    _defStateRef = stateMgr.getActionState<DefensiveState>(AnimationState::Defensive);
    _koStateRef = stateMgr.getActionState<KnockedOutState>(AnimationState::Knocked);
}

void ActionIdleState::handleInput(Animable& a) {
    // Certain actions cannot be applied while jumping

    // Transition to Punch based on input but not if we are jumping
    if (!a.input.is_jumping && a.input.punch) {
        a.setActionState(*_punchStateRef);
    }
    else if (a.input.punchStick) {
        a.setActionState(*_punchStickStateRef);
    }
    else if (!a.input.is_shooting && a.input.shoot) {
        a.setActionState(*_shootStateRef);
    }
    else if (!a.input.is_jumping && a.input.kick) {
        a.setActionState(*_kickStateRef);
    }
    else if (!a.input.is_jumping && a.input.kick2) {
        a.setActionState(*_kick2StateRef);
    }
    else if (!a.input.is_jumping && a.input.defensive) {
        a.setActionState(*_defStateRef);
    }
    else if (a.input.knockedout) {
        a.setActionState(*_koStateRef);
    }
}

void ActionIdleState::update(Animable& a, float dt) {
}

void ActionIdleState::enter(Animable& a) {
}

void PunchState::defineDependencies(StateManager& stateMgr) {
    ActionState::defineDependencies(stateMgr);
}

void PunchState::handleInput(Animable& a) {}

void PunchState::update(Animable& a, float dt) {
    // Is Last Frame?
    if (ActionState::playOnce(a)) {
        a.setActionState(*_idleStateRef);
        a.setMovementState(*_idleMovementStateRef);
    }
    a.resetPosition(); 
}

void PunchState::enter(Animable& a) {
    a.setAnimationState(AnimationState::Punch);
}

void PunchStickState::defineDependencies(StateManager& stateMgr) {
    ActionState::defineDependencies(stateMgr);
}

void PunchStickState::handleInput(Animable& a) {}

void PunchStickState::update(Animable& a, float dt) {
    // Is Last Frame?
    if (ActionState::playOnce(a)) {
        a.setActionState(*_idleStateRef);
        a.setMovementState(*_idleMovementStateRef);
    }
    a.resetPosition(); 
}

void PunchStickState::enter(Animable& a) {
    a.setAnimationState(AnimationState::PunchStick);
}

void ShootState::defineDependencies(StateManager& stateMgr) {
    ActionState::defineDependencies(stateMgr);
}

void ShootState::handleInput(Animable& a) {}

void ShootState::update(Animable& a, float dt) {
    // Is Last Frame?
    if (ActionState::playOnce(a)) {
        a.setActionState(*_idleStateRef);
        a.setMovementState(*_idleMovementStateRef);
    }
    a.resetPosition(); 
}

void ShootState::enter(Animable& a) {
    a.setAnimationState(AnimationState::Shoot);
}


void KickState::defineDependencies(StateManager& stateMgr) {
    ActionState::defineDependencies(stateMgr);
}

void KickState::handleInput(Animable& a) {}

void KickState::update(Animable& a, float dt) {
    // Is Last Frame?
    if (ActionState::playOnce(a)) {
        a.setActionState(*_idleStateRef);
        a.setMovementState(*_idleMovementStateRef);
    }
    a.resetPosition(); 
}

void KickState::enter(Animable& a) {
    a.setAnimationState(AnimationState::Kick);
}


void Kick2State::defineDependencies(StateManager& stateMgr) {
    ActionState::defineDependencies(stateMgr);
}

void Kick2State::handleInput(Animable& a) {}

void Kick2State::update(Animable& a, float dt) {
    // Is Last Frame?
    if (ActionState::playOnce(a)) {
        a.setActionState(*_idleStateRef);
        a.setMovementState(*_idleMovementStateRef);
    }
    a.resetPosition(); 
}

void Kick2State::enter(Animable& a) {
    a.setAnimationState(AnimationState::Kick2);
}


void DefensiveState::defineDependencies(StateManager& stateMgr) {
    ActionState::defineDependencies(stateMgr);
}

void DefensiveState::handleInput(Animable& a) {}

void DefensiveState::update(Animable& a, float dt) {
    // Is Last Frame?
    if (ActionState::playOnce(a)) {
        a.setActionState(*_idleStateRef);
        a.setMovementState(*_idleMovementStateRef);
    }
    a.resetPosition(); 
}

void DefensiveState::enter(Animable& a) {
    a.setAnimationState(AnimationState::Defensive);
}


void KnockedOutState::defineDependencies(StateManager& stateMgr) {
    ActionState::defineDependencies(stateMgr);
}

void KnockedOutState::handleInput(Animable& a) {}

void KnockedOutState::update(Animable& a, float dt) {
    // Is Last Frame?
    if (ActionState::playOnce(a)) {
        a.setActionState(*_idleStateRef);
        a.setMovementState(*_idleMovementStateRef);
    }
    a.resetPosition(); 
}

void KnockedOutState::enter(Animable& a) {
    a.setAnimationState(AnimationState::Knocked);
}