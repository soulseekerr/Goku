#pragma once

#include <memory>
#include <string_view>
#include <fstream>
#include <exception>
#include <format>

#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h
#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

// #include "core/vector.h"
#include "log_levels.h"
#include "logger.h"
#include "sprite2d.h"
#include "gamewindow.h"
#include "animable.h"

namespace soul {

/**
 * @brief Base class for all GUI windows
 * GuiWindow derived classes are used to debug in real-time the data of the game
 */
class GuiWindow {
public:
    GuiWindow() {}
    virtual ~GuiWindow() {}
    virtual void render(GameWindow& gw) = 0;
};

/**
 * @brief GuiDebugLog class
 * Window for logs displayed on screen using ImGui
 */
class GuiDebugLog : public GuiWindow {
private:
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    int                 RowCount {0};

public:
    GuiDebugLog() : GuiWindow() {
        AutoScroll = true;
        clear();
    }

    virtual ~GuiDebugLog() {}

    void render(GameWindow& gw) override;

    void clear() {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
        RowCount = 0;
    }

    void addLog(const char* fmt, ...) IM_FMTARGS(2) {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }
};

/**
 * @brief GuiAnimableStates class
 * Window for the states of an animable object
 */
class GuiAnimableStates : public GuiWindow {
private:
    std::shared_ptr<soul::Animable> animable;
public:
    int tmp_pos_x, tmp_pos_y;
    int tmp_jumpForce, tmp_gravityForce;

    explicit GuiAnimableStates(std::shared_ptr<soul::Animable> s) : GuiWindow(), animable(s) {
        init();
    }
    virtual ~GuiAnimableStates() {}
    void init();
    void render(GameWindow& gw) override;
};

/**
 * @brief GuiSpriteTest class
 * Window for testing a sprite
 */
class GuiSpriteTest : public GuiWindow {
private:
    shared_ptr<Sprite2d> _sprite;
    Vector2i _pos, _size;
    Vector2f _scale;
    Vector2i _spritePosition;

public:
    GuiSpriteTest(const std::string& spriteTestFile) : GuiWindow(), _sprite(make_shared<soul::Sprite2d>("SpriteTest")) {
        init(spriteTestFile);
    }
    virtual ~GuiSpriteTest() {}

    void init(const std::string& spriteFilename);
    void render(GameWindow& gw) override;
};

/**
 * @brief LoggerGui class
 * Logger for the GUI window
 */
class LoggerGui : public ILogger {

public:
    LoggerGui(const LOG_LEVEL level, shared_ptr<GuiDebugLog>& p_guiDebugLog);
    virtual ~LoggerGui();

    void write(const LOG_LEVEL level, const string& s) override;

private:
    // Instance of the Logger GUI
    shared_ptr<GuiDebugLog>& guiDebugLog;
};

} // namespace soul