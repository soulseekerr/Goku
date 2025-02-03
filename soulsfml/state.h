#pragma once

#include "core/singleton.h"
#include "spriteanimation.h"

namespace soul {

class Animable;
class StateManager;
class WalkState;
class JumpState;

/**
 * @brief Movement State of an Animable object
 * MovementState is an abstract class that defines the interface for all states
 *  defined for movements of an Animable object
 */
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
    void jumpY(Animable& a, float dt);
    bool landY(Animable& a, float dt);

    soul::LoggerManager& logManager = soul::LoggerManager::getInstance();
};

class ActionIdleState;

class ActionState {
public:
    ActionState() = default;
    virtual ~ActionState() = default;

    virtual void defineDependencies(StateManager& stateMgr) = 0;

    virtual void handleInput(Animable& a) = 0;
    virtual void update(Animable& a, float dt) = 0;
    virtual void enter(Animable& a) = 0;

    soul::LoggerManager& logManager = soul::LoggerManager::getInstance();

    bool playOnce(Animable& a);

protected:
    // Direct References to other states accessed by this state
    ActionIdleState* _idleStateRef;
};

/** 
 * @brief StateManager of Animable objects
 * StateManager is a class that manages all the states of an Animable object
 */
class StateManager {
private:
    // Mapping of AnimationState to MovementState
    std::map<AnimationState, MovementState*> mapMovementStates;
    // Mapping of AnimationState to ActionState
    std::map<AnimationState, ActionState*> mapActionStates;

public:
    StateManager() = default;
    virtual ~StateManager() {
        for (auto& state : mapMovementStates) {
            delete state.second;
        }
        for (auto& state : mapActionStates) {
            delete state.second;
        }
    }

public:
    // Register a MovementState for the given AnimationState
    template <typename T>
    T* registerMovement(AnimationState state) {
        static_assert(std::is_base_of<MovementState, T>::value, "T must derive from MovementState");
        mapMovementStates[state] = new T();
        return getMovementState<T>(state);
    }

    // Get MovementState of the given AnimationState
    template <typename T>
    T* getMovementState(AnimationState state) {
        return dynamic_cast<T*>(mapMovementStates[state]);
    }

    // Register an ActionState for the given AnimationState
    template <typename T>
    T* registerAction(AnimationState state) {
        static_assert(std::is_base_of<ActionState, T>::value, "T must derive from ActionState");
        mapActionStates[state] = new T();
        return getActionState<T>(state);
    }

    // Get the ActionState of the given AnimationState
    template <typename T>
    T* getActionState(AnimationState state) {
        return dynamic_cast<T*>(mapActionStates[state]);
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
    // Direct References to other states accessed by this state
    WalkState* _walkStateRef;
    JumpState* _jumpStateRef;
};

class JumpState : public MovementState {
public:
    JumpState() = default;

    void defineDependencies(StateManager& stateMgr) override;

    void handleInput(Animable& a) override;
    void update(Animable& a, float dt) override;
    void enter(Animable& a) override;

private:
    // Direct References to other states accessed by this state
    IdleState* _idleStateRef;
    WalkState* _walkStateRef;
};

class WalkState : public MovementState {
public:
    WalkState()  = default;

    void defineDependencies(StateManager& stateMgr) override;

    void handleInput(Animable& a) override;
    void update(Animable& a, float dt) override;
    void enter(Animable& a) override;

private:
    // Direct References to other states accessed by this state
    JumpState* _jumpStateRef;
    IdleState* _idleStateRef;
};

class PunchState;

class ActionIdleState : public ActionState {
public:
    ActionIdleState() = default;
    virtual ~ActionIdleState() = default;

    void defineDependencies(StateManager& stateMgr) override;

    virtual void handleInput(Animable& a) override;
    virtual void update(Animable& a, float dt) override;
    virtual void enter(Animable& a) override;

private:
    // Direct References to other states accessed by this state
    PunchState* _punchStateRef;
};

class PunchState : public ActionState {
public:
    PunchState() = default;

    void defineDependencies(StateManager& stateMgr) override;

    void handleInput(Animable& a) override;
    void update(Animable& a, float dt) override;
    void enter(Animable& a) override;
};

} // namespace soul