
#include "state.h"
#include "animable.h"

using namespace soul;

void MovementState::translateX(Animable& a, float dt) {

    if (a.input.left) {
        logManager.logDebug("MovementState::translateX LEFT");
        a._sprite->transform.direction = -1;
        a._sprite->velocity.x = -a._sprite->transform.moveSpeed; // Move left
    }
    else if (a.input.right) {
        logManager.logDebug("MovementState::translateX RIGHT");
        a._sprite->transform.direction = 1;
        a._sprite->velocity.x = a._sprite->transform.moveSpeed; // Move right
        a._sprite->setScaleFactor(a._sprite->scale.x, a._sprite->scale.y);
    }
    else {
        // Never seen as we switch to another state if we dont input
        logManager.logDebug("MovementState::translateX NO MOVE");
        // NEVER CALLED!
        // a.setVelocityX(0.0f); // Stop horizontal movement
    }

    // Update player position based on velocity
    logManager.logDebug("MovementState::translateX: incrPositionX by dt {} speed {} velocity {} pos {}",  
    dt, a._sprite->transform.moveSpeed, a._sprite->velocity.x, a._sprite->position.x);

    a.incrPositionX(a._sprite->velocity.x);

    // Set Sprite factor if we are facing left or right
    a._sprite->setScaleFactor(a._sprite->transform.direction * a._sprite->scale.x, a._sprite->scale.y);
}

// void MovementState::jumpY(Animable& a, float dt) {
//     // Increase jump velocity in negative terms 
//     // because screen coords we go down in positive
//     a._sprite->velocity.y = -a._sprite->transform.jumpForce; 
//     // Update player position based on velocity
//     a._sprite->position.y += a._sprite->velocity.y * dt;

//     a.input.is_jumping = true;
//     a.input.is_on_ground = false;
// }

// bool MovementState::landY(Animable& a, float dt) {

    // if (a.input.is_jumping) {
        // If we are in jump mode we apply gravity in positive terms
        // because screen coords we go down in positive
        // a._sprite->velocity.y += a._sprite->transform.gravityForce * dt; // Apply gravity
    // }

    // we are on a platform 
    // if (a.input.is_on_ground) {
    //     a._sprite->velocity.y = 0;
    //     a.input.is_jumping = false;
    //     return true;
    // }

    // Update player position based on velocity
    // a._sprite->position.y += a._sprite->velocity.y * dt;

    // We are in jump mode and we are back on the ground, applying gravity
    // if (a._sprite->position.y >= a._initialPositionY) {
    //     a._sprite->position.y = a._initialPositionY;
    //     a._sprite->velocity.y = 0;
    //     a.input.is_jumping = false;
    //     return true;
    // }

//     return false;
// }

void IdleState::defineDependencies(StateManager& stateMgr) {
    _walkStateRef = stateMgr.getMovementState<WalkState>(AnimationState::Walk);
    // _jumpStateRef = stateMgr.getMovementState<JumpState>(AnimationState::Jump);
}

void IdleState::handleInput(Animable& a) {
    // Transition to Walk, Jump, or Punch based on input
    if (a.input.left || a.input.right) {
        a.setMovementState(*_walkStateRef);
    }
    else {
        a.setVelocityX(0.0f); // Stop horizontal movement
    }
    // if (a.input.up) {
    //     a.setMovementState(*_jumpStateRef);
    // }
}

void IdleState::update(Animable& a, float dt) {
    logManager.logDebug("IdleState::update");
    // No special update logic for Idle (the animation will auto-play)
}

void IdleState::enter(Animable& a) {
    logManager.logDebug("IdleState::enter");
    a.setAnimationState(AnimationState::Idle); // Set idle animation
}

// void JumpState::defineDependencies(StateManager& stateMgr) {
//     _walkStateRef = stateMgr.getMovementState<WalkState>(AnimationState::Walk);
//     _idleStateRef = stateMgr.getMovementState<IdleState>(AnimationState::Idle);
// }

// void JumpState::handleInput(Animable& a) {
//     // During the jump, inputs might be ignored or limited
//     // Cannot combine with other Movement state 
// }

// void JumpState::update(Animable& a, float dt) {
//     // Update jump logic (e.g., gravity, position updates)
//     // After landing, transition back to Idle or Walk
    
//     if (a.input.left) {
//         translateX(a, dt);
//     }

//     if (a.input.right) {
//         translateX(a, dt);
//     }

//     if (!a.input.is_jumping) {
//         jumpY(a, dt);
//         a.resetPosition();
//         return;
//     }
    
//     // Player is jumping here and has to land on the ground
//     if (landY(a, dt)) {
//         a.setMovementState(*_idleStateRef);
//     }

//     a.resetPosition();
// }

// void JumpState::enter(Animable& a) {
//     a.setAnimationState(AnimationState::Jump); // Set jump animation
// }

void WalkState::defineDependencies(StateManager& stateMgr) {
    // _jumpStateRef = stateMgr.getMovementState<JumpState>(AnimationState::Jump);
    _idleStateRef = stateMgr.getMovementState<IdleState>(AnimationState::Idle);
}

void WalkState::handleInput(Animable& a) {
    // if not moving
    if (!a.input.left && !a.input.right) {
        a.setMovementState(*_idleStateRef);
    }
    
    // if (a.input.up) {
    //     a.setMovementState(*_jumpStateRef);
    // }
}

void WalkState::update(Animable& a, float dt) {
    logManager.logDebug("WalkState::update");
    // Update walk logic (e.g., moving the player position)
    // The animation update is handled by the AnimationSet
    translateX(a, dt);

    a.resetPosition();
}

void WalkState::enter(Animable& a) {
    logManager.logDebug("WalkState::enter");
    a.setAnimationState(AnimationState::Walk); // Set walk animation
}