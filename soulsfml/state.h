#pragma once

#include "core/singleton.h"
#include "spriteanimation.h"

namespace soul {

class Animable;
class StateManager;
class WalkState;
// class JumpState;

class MovementState {
public:
    MovementState() = default;
    virtual ~MovementState() = default;

    virtual void defineDependencies(StateManager& stateMgr) = 0;

    virtual void handleInput(Animable& a) = 0;
    virtual void update(Animable& a, float dt) = 0;
    virtual void enter(Animable& a) = 0;

protected:
    void translateX(Animable& a, float dt);
    // void jumpY(Animable& a, float dt);
    // bool landY(Animable& a, float dt);

    soul::LoggerManager& logManager = soul::LoggerManager::getInstance();
};

class StateManager {
private:
    std::map<AnimationState, MovementState*> mapMovementStates;

public:
    StateManager() = default;
    virtual ~StateManager() {
        for (auto& state : mapMovementStates) {
            delete state.second;
        }
    }

public:
    template <typename T>
    T* registerMovement(AnimationState state) {
        static_assert(std::is_base_of<MovementState, T>::value, "T must derive from MovementState");
        mapMovementStates[state] = new T();
        return getMovementState<T>(state);
    }

    template <typename T>
    T* getMovementState(AnimationState state) {
        return dynamic_cast<T*>(mapMovementStates[state]);
    }
};

class IdleState : public MovementState {
public:
    IdleState() = default;

    void defineDependencies(StateManager& stateMgr) override;

    void handleInput(Animable& a) override;
    void update(Animable& a, float dt) override;
    void enter(Animable& a) override;

private:
    WalkState* _walkStateRef;
    // JumpState* _jumpStateRef;
};

// class JumpState : public MovementState {
// public:
//     JumpState() = default;

//     void defineDependencies(StateManager& stateMgr) override;

//     void handleInput(Animable& a) override;
//     void update(Animable& a, float dt) override;
//     void enter(Animable& a) override;

// private:
//     IdleState* _idleStateRef;
//     WalkState* _walkStateRef;
// };

class WalkState : public MovementState {
public:
    WalkState()  = default;

    void defineDependencies(StateManager& stateMgr) override;

    void handleInput(Animable& a) override;
    void update(Animable& a, float dt) override;
    void enter(Animable& a) override;

private:
    // JumpState* _jumpStateRef;
    IdleState* _idleStateRef;
};

} // namespace soul