#pragma once

#include "sprite2d.h"
#include "core/vector_t.h"

#include <map>

namespace soul {

struct Frame {
    int x;
    int y;
    int width;
    int height;
    float displayTimeSeconds;
};

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
    Vector_t<Frame> _frames;
    int _currentFrameIndex {0};
    float _currentFrameTime {0.0f};
};

enum class AnimationState {
    None, 
    Idle, 
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
    Patrol,
    Chase,
    Attack
};

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