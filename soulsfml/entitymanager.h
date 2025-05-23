#pragma once

#include <memory>
#include <vector>
#include <map>
#include <assert.h>
#include <mutex>
#include <optional>

#include "singleton.h"
#include "logger.h"
#include "entity.h"

namespace soul {

/**
 * @brief EntityManager class
 * ECS like entity manager for managing entities in the game
 */
class EntityManager : public SingletonT<EntityManager> {

    MAKE_SINGLETON(EntityManager)

private:
    LoggerManager& logManager = LoggerManager::getInstance();

    std::vector<std::shared_ptr<Entity>> _entities;
    std::vector<std::shared_ptr<Entity>> _entitiesToAdd;
    std::map<std::string, std::vector<std::shared_ptr<Entity>>> _mapEntities;

public:
    _ALWAYS_INLINE_ size_t getTotalEntities() const { return _entities.size(); }

    _ALWAYS_INLINE_ std::vector<std::shared_ptr<Entity>>& getEntities() { return _entities; }

    std::vector<std::shared_ptr<Entity>>& getEntities(const std::string& tag);

    void addEntity(std::shared_ptr<Entity>& e);

    void update();

private:
    void removeDeadEntities();
};

} // namespace soul