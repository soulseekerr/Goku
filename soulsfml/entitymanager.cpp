
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
    // Create the entity
    _totalEntities++;

    // // Add the entity to the list created to prevent iterator invalidation
    logManager.logDebug("Adding entity instance {} {}", e->ID(), e->tag());
    _entitiesToAdd.push_back(e);
}

void EntityManager::removeDeadEntities() {

    std::vector<std::tuple<int, std::string>> dead_entities_positions;

    int index = 0;
    for (auto& e : _entities) {
        if (!e->isActive()) {
            // std::cout << "Add dead entity ID " << e->id() << " Tag " << e->tag() << endl;
            dead_entities_positions.push_back({index, e->tag()});
        }
        index++;
    }

    for (auto t : dead_entities_positions) {
        // std::cout << "Deleting dead entity " << std::get<1>(t) << endl;
        _entities.erase(_entities.begin() + std::get<0>(t));
    }
    
    for (auto t : dead_entities_positions)
        _mapEntities.erase(std::get<1>(t));
}

void EntityManager::update()
{
    // Add the new entities in the containers
    for (auto e : _entitiesToAdd)
    {
        // std::cout << "Add Entity " << e->tag() << " to map entities" << endl; 
        _entities.push_back(e);
        _mapEntities[e->tag()].push_back(e);
    }
    _entitiesToAdd.clear();

    // Need to delete all entities declared as dead
    // store the index and the tag for each dead entity
    // then iterate through them and erase
    removeDeadEntities();
}