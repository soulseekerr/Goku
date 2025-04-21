
#include "entitymanager.h"

#include <iostream>
#include <fstream>

using namespace soul;

std::vector<std::shared_ptr<soul::Entity>>& EntityManager::getEntities(const std::string& tag) {
    std::map<string, vector<std::shared_ptr<soul::Entity>>>::iterator it = _mapEntities.find(tag);
    if (it == _mapEntities.end()) {
        // we create a new empty vector
        std::cout << "Creating new Vector " << tag << endl;
        std::vector<std::shared_ptr<soul::Entity>> v;
        _mapEntities[tag] = v;

        it = _mapEntities.find(tag);
        if (it == _mapEntities.end())
            throw std::invalid_argument("could not find entity vector newly created with tag " + tag);
    }

    return it->second;
}

void EntityManager::addEntity(std::shared_ptr<soul::Entity>& e) {
    // // Add the entity to the list created to prevent iterator invalidation
    logManager.logDebug("Adding entity instance ID:{} tag:{} IsActive:{}", e->ID(), e->tag(), e->isActive());
    _entitiesToAdd.push_back(e);
}

void EntityManager::removeDeadEntities() {
    // Remove dead entities from _entities
    size_t count_before = _entities.size();
    _entities.erase(std::remove_if(_entities.begin(), _entities.end(),
        [](const std::shared_ptr<Entity>& e) {
            return (!e->isActive());
        }), _entities.end());

    auto diff = count_before - _entities.size();
    if (diff > 0) {
        logManager.logDebug("{} entities removed.", diff);
    }

    // Remove dead entities from _mapEntities
    for (auto it = _mapEntities.begin(); it != _mapEntities.end(); ) {
        it->second.erase(std::remove_if(it->second.begin(), it->second.end(),
            [](const std::shared_ptr<Entity>& e) {
                return (!e->isActive());
            }), it->second.end());

        // If no more entities remain under a tag, remove the tag
        if (it->second.empty()) {
            it = _mapEntities.erase(it);
        } else {
            ++it;
        }
    }
}

void EntityManager::update()
{
    // Add the new entities in the containers
    for (auto e : _entitiesToAdd) {
        std::cout << "Add Entity " << e->tag() << " to map entities" << endl; 
        _entities.push_back(e);
        _mapEntities[e->tag()].push_back(e);
    }
    _entitiesToAdd.clear();

    // Need to delete all entities declared as dead
    // store the index and the tag for each dead entity
    // then iterate through them and erase
    removeDeadEntities();
}