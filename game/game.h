#pragma once

#include "gamewindow.h"
#include "state.h"
#include "core/logger.h"
#include "core/utils.h"
#include "animable.h"
#include "gui.h"

class Game
{
private:
    bool running_;
    soul::GameWindow& gw_;
    soul::StateManager stateManager_;
    std::shared_ptr<soul::Animable> player_;

#ifdef DEBUGMODE
    std::shared_ptr<soul::GuiAnimableStates> guiStates_;
    std::shared_ptr<soul::GuiSpriteTest> guiSpriteTest_;
    std::shared_ptr<soul::GuiDebugLog> guiDebugLog_;
#endif

public:
    Game();
    ~Game();

    int run();

    bool isRunning() const { return running_; }

    static Game& getInstance()
    {
        static Game instance;
        return instance;
    }
};
