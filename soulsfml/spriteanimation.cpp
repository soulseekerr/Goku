
#include "spriteanimation.h"

using namespace soul;

SpriteAnimation::SpriteAnimation() {}

SpriteAnimation::~SpriteAnimation() {}

void SpriteAnimation::addFrame(int x, int y, int width, int height, float frameTime) {
    Frame data;
    // data.id = textureID;
    data.x = x;
    data.y = y;
    data.width = width;
    data.height = height;
    data.displayTimeSeconds = frameTime;
    
    _frames.push_back(data);
}

bool SpriteAnimation::updateFrame(float dt) {
    if(_frames.size() == 0)
        return false;

    _currentFrameTime += dt;

    if(_currentFrameTime >= _frames[_currentFrameIndex].displayTimeSeconds) {
        _currentFrameTime = 0.f;
        incrementFrame();
        return true;
    } 
    return false;
}

void SpriteAnimation::incrementFrame() {
    _currentFrameIndex = (_currentFrameIndex + 1) % _frames.size();
}

const Frame& SpriteAnimation::getCurrentFrame() const { return _frames[_currentFrameIndex]; }

bool SpriteAnimation::isLastFrame() const {
    // std::cout << "currentFrameIndex=" << currentFrameIndex << " / " << frames.size() <<  std::endl;
    return _currentFrameIndex >= _frames.size() - 1 ? true : false;
}

void SpriteAnimation::reset() {
    _currentFrameIndex = 0;
    _currentFrameTime = 0.0f;
}


AnimationSet::AnimationSet(std::shared_ptr<soul::Sprite2d>& sprite) : _spriteRef(sprite) {}

AnimationSet::~AnimationSet() {}

// Set current animation states.
void AnimationSet::setAnimationState(AnimationState state) {
    // We only set an animation of it is not already 
    // our current animation.
    if (_currentAnimation.first == state) {
        return;
    }
        
    auto animation = _animations.find(state);

    if (animation != _animations.end()) {
        _currentAnimation.first = animation->first;
        _currentAnimation.second = animation->second;            
        _currentAnimation.second->reset();
    }
} 
	
// Returns current animation state.
const AnimationState& AnimationSet::getAnimationState() const {
    // Returns out current animation state. We can use this 
	// to compare the objects current state to a desired state.
    return _currentAnimation.first;
}

// Add animation to object. We need its state as well 
// so that we can switch to it.
void AnimationSet::addAnimation(AnimationState state, std::shared_ptr<SpriteAnimation> animation) {
    // auto inserted = 
    _animations.insert(std::make_pair(state, animation));
    
    if (_currentAnimation.first == AnimationState::None)
        setAnimationState(state);
}

void AnimationSet::update(float dt) {

    if(_currentAnimation.first == AnimationState::None) return;

    // is it a new frame in the current animation
    auto newFrame = _currentAnimation.second->updateFrame(dt);
    if(newFrame) {
        const Frame& data = _currentAnimation.second->getCurrentFrame();
        // _spriteRef->setTextureRect(sf::IntRect(data.x, data.y, data.width, data.height));
        _spriteRef->setTextureRect(soul::Vector2i(data.x, data.y), soul::Vector2i(data.width, data.height));
    }
}

const std::shared_ptr<SpriteAnimation>& AnimationSet::getCurrentAnimation() const {
    return _currentAnimation.second;
}