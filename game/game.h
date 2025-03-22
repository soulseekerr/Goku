#pragma once

#include "gamewindow.h"
#include "state.h"
#include "logger.h"
#include "utils.h"
#include "animable.h"
#include "gui.h"
#include "scene.h"

/**
 * @brief The Game class
 *  The main class that runs the game loop
 * 
 * Attributes:
 * - running_: Flag to indicate if the game is running
 * - gw_: Reference to the GameWindow instance
 * - stateManager_: State manager
 * - player_: Player instance
 * - scene_: Scene instance
 */
class Game
{
private:
    bool running_;
    // Game window
    soul::GameWindow& gw_;
    // State manager
    soul::StateManager stateManager_;
    // Player instance
    std::shared_ptr<soul::Player> player_;
    // Scene instance
    std::unique_ptr<soul::Scene> scene_;

#ifdef DEBUGMODE
    std::shared_ptr<soul::GuiAnimableStates> guiStates_;
    std::shared_ptr<soul::GuiSpriteTest> guiSpriteTest_;
    std::shared_ptr<soul::GuiDebugLog> guiDebugLog_;
#endif

public:
    Game();
    ~Game();

    [[nodiscard]] int run();

    _ALWAYS_INLINE_ bool isRunning() const { return running_; }

    // Singleton
    static Game& getInstance()
    {
        static Game instance;
        return instance;
    }
};
