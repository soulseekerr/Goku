#pragma once

#include "logger.h"
#include "canvas.h"
#include "gui.h"
#include "entitymanager.h"
#include "assetmanager.h"
#include "pathmanager.h"

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

public:
    // Game Window
    GameWindow& gw = soul::GameWindow::getInstance();
    // Container for entities
    EntityManager&  entities = EntityManager::getInstance();
    // Container for Assets
    AssetManager&  assets = AssetManager::getInstance();
    // Canvas 2D layer
    std::shared_ptr<Canvas> canvasLayer;
    // Always a Player in the Scene
    std::shared_ptr<Player> player;

public:
    /**
     * @brief Constructs a Scene with a given player instance.
     *
     * @param p The player's instance in a shared smart pointer.
     */
    Scene(shared_ptr<Player> p) : player(p) {
        canvasLayer = std::make_shared<Canvas>("Canvas", *this);
    }

    /**
     * @brief Destructor.
     *
     */
    virtual ~Scene() {}

    /**
     * @brief Initialise the objects in the Game instance.
     *
     */
    void initialise();

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
};

} // namespace soul