
#include "core/logger.h"
#include "sprite2d.h"
#include "assetmanager.h"

#include <iostream>
#include <optional>

#include <SFML/Graphics.hpp>

#define DEBUGMODE

#ifdef DEBUGMODE
#define DEFINED_LOG_LEVEL soul::LOG_LEVEL::LOG_DEBUG
#else
#define DEFINED_LOG_LEVEL soul::LOG_LEVEL::LOG_INFO
#endif

int main(int argc, char** argv) {
    auto& logManager = soul::LoggerManager::getInstance();
    logManager.addLogger(make_shared<soul::LoggerConsole>(DEFINED_LOG_LEVEL));
    logManager.log("Starting Sprite Test");

    // Create a window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Sprite Example");
    window.setFramerateLimit(60);

    std::shared_ptr<soul::Sprite2d> sprite;

    auto name = "PlayerSprite";
    auto filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/Kid Goku.png";
    auto isSmooth = true;

    sprite = std::make_shared<soul::Sprite2d>(name);

    try {
        sprite->loadTexture(
            filePath, 
            soul::Vector2f(1.0f, 1.0f), 
            isSmooth);
    }
    catch(soul::TextureException& e) {
       std::cout << "Error: " << e.what() << std::endl;
       exit(-1);
    }

    // Set the initial position of the sprite
    sprite->setPosition(100.f, 50.f);
    sprite->setScale(1.5f, 1.5f);
    sprite->setTextureRect(10, 128, 80, 100);

    // Get the sprite
    auto& spritePtr = sprite->getSprite();

    // Main game loop
    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the sprite
        window.draw(spritePtr);

        // Display what has been drawn
        window.display();
    }

    return 0;
}