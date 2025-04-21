#pragma once

#include "logger.h"
#include "canvas.h"
#include "player.h"
#include "gui.h"
#include "entitymanager.h"
#include "assetmanager.h"
#include "pathmanager.h"
#include "vector2.h"

#include <memory>
#include <vector>

namespace soul {

class GameException;
class Player;

/**
 * @brief Scene class
 * Class that represents a scene in the game.
 * 
 */
class Scene {
private:
    // Logger Manager unique instance
    LoggerManager& logManager = LoggerManager::getInstance();
    // Path Manager
    PathManager& pathManager = PathManager::getInstance();
    // Scene Name
    std::string _name;

public:
    // Game Window
    GameWindow& gw = soul::GameWindow::getInstance();
    // Container for entities
    EntityManager&  entities = EntityManager::getInstance();
    // Container for Assets
    AssetManager&  assets = AssetManager::getInstance();
    // Always a Player in the Scene
    std::unique_ptr<Player> player;
    // Canvas 2D layer
    std::shared_ptr<Canvas> canvasLayer;

public:
    /**
     * @brief Constructs a Scene with a given player instance.
     *
     * @param name Scene name.
     */
    explicit Scene(const std::string& name);

    /**
     * @brief Destructor.
     *
     */
    virtual ~Scene() {}

    /**
     * @brief Update the scene in the game loop.
     *
     * @param dt time delta in the game loop.
     */
    void update(float dt);

    /**
     * @brief Render the scene in the game loop.
     *
     */
    void render();

private:
    /**
     * @brief Handle collision between player and platform.
     *
     * @param playerBounds The bounding box of the player.
     * @param platformBounds The bounding box of the platform.
     */
    void _handleCollision(soul::Rect<float>& playerBounds, const soul::Rect<float>& platformBounds);
};

} // namespace soul