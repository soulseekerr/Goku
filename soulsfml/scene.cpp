
#include "scene.h"
#include "logger.h"
#include "tile.h"
#include "loaders.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace soul;

Scene::Scene(const std::string& name) : _name(name) {

    const std::string playerFile = pathManager.getFilePath(PathManager::FileType::Data, "player_goku.json")->string();
    player = Loader<Player>::load(playerFile);

    const std::string canvasFile = pathManager.getFilePath(PathManager::FileType::Data, "canvas.json")->string();
    canvasLayer = SceneLoader<Canvas>::load(canvasFile, *this);
}

void Scene::update(float dt) {
    // Hanlde Inputs in the Animable object
    // Update the states and animation associated to the current movement or action
    player->update(dt);
    canvasLayer->update(dt);

    entities.update();
}

void Scene::render() {

    for (auto& entity : entities.getEntities()) {
        if (entity->isActive()) {
            entity->render();
        }
    }

    player->render();
    canvasLayer->render();
}

void Scene::_handleCollision(soul::Rect<float>& playerBounds, const soul::Rect<float>& platformBounds) {

    if (Collision::isAABBIntersection(playerBounds, platformBounds)) {
        // logManager.log("Collision detected!");

        // Calculate collision depth
        float overlapLeft = playerBounds.left + playerBounds.width - platformBounds.left;
        float overlapRight = platformBounds.left + platformBounds.width - playerBounds.left;
        float overlapTop =playerBounds.top + playerBounds.height - platformBounds.top;
        float overlapBottom = platformBounds.top + platformBounds.height - playerBounds.top;

        // Find the smallest overlap to resolve collision
        bool fromLeft = std::abs(overlapLeft) < std::abs(overlapRight);
        bool fromTop = std::abs(overlapTop) < std::abs(overlapBottom);

        float minOverlapX = fromLeft ? overlapLeft : overlapRight;
        float minOverlapY = fromTop ? overlapTop : overlapBottom;

        // Resolve collision based on the smallest overlap
        if (std::abs(minOverlapX) < std::abs(minOverlapY)) {
            // Horizontal collision
            if (fromLeft) {
                player->incrPositionX(-minOverlapX);  // Move player to the left
            } else {
                player->incrPositionX(minOverlapX);   // Move player to the right
            }
        } else {
            // Vertical collision
            if (fromTop) {
                player->incrPositionY(-minOverlapY); // Move player up
                player->input.is_on_ground = true;  // If collided from top, player is on the ground
            } else {
                player->incrPositionY(minOverlapY); // Move player down
            }
        }
    }
}