#pragma once

#include <exception>
#include <memory>
#include <thread>
#include <iostream>
#include <format>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h
#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

#include "singleton.h"
#include "logger.h"

using soul::LoggerManager;

namespace soul {

class GameException : std::exception {
    const char* msg;
public:
    GameException() = delete;
    explicit GameException(const char* p_msg) noexcept : msg(p_msg) {}
    const char* what() const noexcept { return msg; } 
};

/**
 * @brief GameWindow class
 * 
 * Singleton class that manages the game window
 */
class GameWindow : public SingletonT<GameWindow> {

    MAKE_SINGLETON(GameWindow)

public:
    void initialise(
        const std::string& title, 
        unsigned int p_width, 
        unsigned int p_height, 
        bool p_fullscreen, 
        int p_frameRateLimit);

public:
    sf::RenderWindow window;
private:
    bool _fullscreen {false};
    bool _isShutdown {false};
};

} // namespace soul