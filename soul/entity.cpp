
#include "entity.h"
// #include "scene.h"

// Generate a random number generator for entity IDs
soul::RandomNumberGenerator soul::Entity::_gen;

// Constructor: Initialize the entity with a unique ID, tag, and active state
soul::Entity::Entity(const std::string& tag)
    : _tag(tag), _id(_gen.getID()), _active(true) {}

// Get the unique ID of the entity
const uint32_t& soul::Entity::ID() const {
    return _id;
}

// Get the tag of the entity
const std::string& soul::Entity::tag() const {
    return _tag;
}

// Set the active state of the entity
void soul::Entity::setActive(bool active) {
    _active = active;
}