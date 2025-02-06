#pragma once

#include "sprite2d.h"
#include "core/vector_t.h"

#include <map>

namespace soul {

/**
 * @brief Frame struct
 * This struct holds the information for a single frame of an animation
 */
struct Frame {
    int x;
    int y;
    int width;
    int height;
    float displayTimeSeconds;
};

/**
 * @brief SpriteAnimation class
 * This class is responsible for managing a set of frames and displaying them
 * in a sequence to create an animation
 */
class SpriteAnimation {
public:
    SpriteAnimation();
    virtual ~SpriteAnimation();

    void addFrame(int x, int y, int width, int height, float frameTime);

    bool updateFrame(float dt);

    void incrementFrame();

    const Frame& getCurrentFrame() const;

    bool isLastFrame() const;

    void reset();

private:
    // The frames that make up the animation
    Vector_t<Frame> _frames;
    // The frame index we are currently at
    int _currentFrameIndex {0};
    // Current frame time
    float _currentFrameTime {0.0f};
};

enum class AnimationState {
    None, 
    Idle, 
    ActionIdle,
    Walk, 
    Jump, 
    Knocked,
    Punch,
    PunchStick,
    Kick,
    Kick2,
    JumpKick,
    Defensive,
    Shoot,
    ShootFireball,
    Patrol,
    Chase,
    Attack
};

/**
 * @brief AnimationSet class
 * This class is responsible for managing a set of animations and switching between them
 */
class AnimationSet {
public:
    AnimationSet(std::shared_ptr<Sprite2d>& sprite);
    virtual ~AnimationSet();

    // Set current animation states.
    void setAnimationState(AnimationState state);

    const AnimationState& getAnimationState() const;

    // Add animation to object. We need its state as well 
	// so that we can switch to it.
    void addAnimation(AnimationState state, std::shared_ptr<SpriteAnimation> animation);

    void update(float dt);

    const std::shared_ptr<SpriteAnimation>& getCurrentAnimation() const;

private:
    std::shared_ptr<soul::Sprite2d> _spriteRef;

    std::map<AnimationState, std::shared_ptr<SpriteAnimation>> _animations;

    // We store a reference to the current animation so we 
	// can quickly update and draw it.
    std::pair<AnimationState, std::shared_ptr<SpriteAnimation>> _currentAnimation;
};

} // namespace soul