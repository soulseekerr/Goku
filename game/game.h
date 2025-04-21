#pragma once

#include "gamewindow.h"
#include "state.h"
#include "logger.h"
#include "utils.h"
#include "animable.h"
#include "gui.h"
#include "scene.h"
#include "pathmanager.h"

#define GAME_NAME "Goku"
#define GAME_VERSION "0.2.5"
#define GAME_AUTHOR "Soul"
#define GAME_DESCRIPTION "A fighting game with Goku as the main character."

/**
 * @brief The Game class
 *  The main class that runs the game loop
 * 
 * Attributes:
 * - running_: Flag to indicate if the game is running
 * - gw_: Reference to the GameWindow instance
 * - stateManager_: State manager
 * - scene_: Scene instance
 */
class Game
{
private:
    bool _running;
    // Game window
    soul::GameWindow& _gw;
    // State manager
    soul::StateManager _stateManager;
    // Scene instance
    std::unique_ptr<soul::Scene> _scene;
    // Config file
    const std::string _configFile = "config.json";

#ifdef DEBUGMODE
    std::shared_ptr<soul::GuiAnimableStates> _guiStates;
    std::shared_ptr<soul::GuiSpriteTest> _guiSpriteTest;
    std::shared_ptr<soul::GuiDebugLog> _guiDebugLog;
#endif

public:
    soul::PathManager& pathManager = soul::PathManager::getInstance();

    Game();
    ~Game();

    [[nodiscard]] int run();

    _ALWAYS_INLINE_ bool isRunning() const { return _running; }

    // Singleton
    static Game& getInstance()
    {
        static Game instance;
        return instance;
    }
};
