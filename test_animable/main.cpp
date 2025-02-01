
#include "core/logger.h"
#include "assetmanager.h"
#include "animable.h"

#include <iostream>

#include <SFML/Graphics.hpp>


int main(int argc, char** argv) {
    auto& logManager = soul::LoggerManager::getInstance();
    logManager.addLogger(make_shared<soul::LoggerConsole>(soul::LOG_LEVEL::LOG_DEBUG));
    logManager.log("Starting Sprite Animation Test");

    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Animable Objects");
    window.setFramerateLimit(60);
    

    auto name = "PlayerSprite";
    auto filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/Kid Goku.png";

    auto animable = std::make_shared<soul::Animable>(name);

    soul::sSpriteData spriteData = {
        filePath, 
        soul::Vector2f(1.2f, 1.2f), 
        soul::Vector2i(10, 128), 
        soul::Vector2i(80, 102), 
        soul::Vector2i(300, 50), 
        false, 
        soul::Color(0, 0, 0, 0)    
    };

    soul::sTransformScalars scalars;
    scalars.initialVelocityX = 6.0f;
    scalars.gravity = 1700.0f;
    scalars.initialVelocityY = -700.0f;
    scalars.groundY = 450;

    animable->load(spriteData, scalars);
    
    auto animationIdle2 = std::make_shared<soul::SpriteAnimation>();
    // Idle 2nd line
    animationIdle2->addFrame(10, 128, 80, 102, 0.25f);
    animationIdle2->addFrame(100, 128, 80, 102, 0.25f);
    animationIdle2->addFrame(190, 128, 80, 102, 0.25f);
    animationIdle2->addFrame(100, 128, 80, 102, 0.25f);
    animable->addAnimation(soul::AnimationState::Idle, animationIdle2);

    auto& spritePtr = animable->getSprite();

    auto lastTime = std::chrono::high_resolution_clock::now();

    // Main game loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {

                switch (event.key.code) {

                    case sf::Keyboard::Escape:  
                        cout << "Exiting." << endl;
                        window.close();
                        exit(0);
                    break;
                    default: break;
                }
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Update the animation associated to the current movement or action
        animable->updateStates(dt);

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the sprite
        window.draw(spritePtr);

        // Display what has been drawn
        window.display();

        // Sleep to control frame rate (for demonstration)
        // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Cap at ~60 FPS
    }

    return 0;
}