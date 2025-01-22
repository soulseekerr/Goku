
#include "core/logger.h"
#include "assetmanager.h"
#include "animable.h"
#include "state.h"
#include "core/utils.h"

#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

int main(int argc, char** argv) {
    auto& logManager = soul::LoggerManager::getInstance();
    logManager.addLogger(make_shared<soul::LoggerConsole>(soul::LOG_LEVEL::LOG_DEBUG));
    logManager.log("Starting Sprite Animation Test");

    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Animable Object States");
    window.setFramerateLimit(60);

    auto name = "PlayerSprite";
    auto filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/Kid Goku.png";

    soul::StateManager stateManager;
    soul::IdleState* idleState = stateManager.registerMovement<soul::IdleState>(soul::AnimationState::Idle);
    // soul::JumpState* jumpState = stateManager.registerMovement<soul::JumpState>(soul::AnimationState::Jump);
    soul::WalkState* walkState = stateManager.registerMovement<soul::WalkState>(soul::AnimationState::Walk);

    idleState->defineDependencies(stateManager);
    // jumpState->defineDependencies(stateManager);
    walkState->defineDependencies(stateManager);

    auto animable = std::make_shared<soul::Animable>(name);

    soul::sSpriteData spriteData = {
        filePath, // Filename for the image file
        soul::Vector2f(1.2f, 1.2f), // Scaling the sprite from the image
        soul::Vector2i(10, 128), // Initial px position in the Sprite sheet
        soul::Vector2i(80, 102), // Initial px size in the Sprite sheet
        soul::Vector2i(300, 300), // Coordinates px of screen as they are defined 0,0 from top left.
        false, // Filter Transparency By Color
        soul::Color(0, 0, 0, 0) // Filtering Color
    };

    auto windowPositionY = 300;

    soul::sTransformScalars scalars;
    scalars.moveSpeed = 6.0f;
    scalars.gravityForce = 0.0f;
    scalars.jumpForce = 0.0f;

    animable->load(windowPositionY, spriteData, scalars);
    
    // Idle 2nd line
    auto animIdle = std::make_shared<soul::SpriteAnimation>();
    animIdle->addFrame(10, 128, 80, 102, 0.25f);
    animIdle->addFrame(100, 128, 80, 102, 0.25f);
    animIdle->addFrame(190, 128, 80, 102, 0.25f);
    animIdle->addFrame(100, 128, 80, 102, 0.25f);
    animable->addAnimation(soul::AnimationState::Idle, animIdle);

    // Walk
    auto animWalk = std::make_shared<soul::SpriteAnimation>();
    animWalk->addFrame(645, 235, 110, 90, 0.05f);
    animable->addAnimation(soul::AnimationState::Walk, animWalk);

    // Jump 6th line
    // auto animJump = std::make_shared<soul::SpriteAnimation>();
    // animJump->addFrame(190, 550, 80, 115, 0.10f);
    // animJump->addFrame(190, 550, 80, 115, 0.10f);
    // animJump->addFrame(190, 550, 80, 115, 0.10f);
    // animJump->addFrame(190, 550, 80, 115, 0.10f);
    // animJump->addFrame(460, 550, 80, 115, 0.10f);
    // animJump->addFrame(460, 550, 80, 115, 0.10f);
    // animJump->addFrame(190, 550, 80, 115, 0.10f);
    // animable->addAnimation(soul::AnimationState::Jump, animJump);

    animable->setAnimationState(soul::AnimationState::Idle);
    animable->setMovementState(*stateManager.getMovementState<soul::IdleState>(soul::AnimationState::Idle));

    sf::Clock deltaClock;
    auto lastTime = std::chrono::high_resolution_clock::now();

    sf::Font font;
    sf::Text text;

    const string fontFilename = "/Users/soulseeker/Projects/cpp_projects/sfml_sparky/build/bin/fonts/MontereyFLF-Bold.ttf";
    font.loadFromFile(fontFilename);

    text.setFont(font);
    text.setFillColor(sf::Color(20, 25, 25));
    text.setCharacterSize(20);
   
    auto text_y = window.getSize().y/20 - (float)text.getCharacterSize();
    text.setPosition(window.getSize().x - 150, text_y);

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
                    
                    case sf::Keyboard::Left:  animable->input.left = true; break;
                    case sf::Keyboard::Right: animable->input.right = true; break;
                    // case sf::Keyboard::Up: animable->input.up = true; break;
                    // case sf::Keyboard::Down: animable->input.down = true; break;
                    
                    default: break;
                }
            }
            
            if (event.type == sf::Event::KeyReleased) {

                switch (event.key.code) {

                    case sf::Keyboard::Left:  animable->input.left = false; break;
                    case sf::Keyboard::Right: animable->input.right = false; break;
                    // case sf::Keyboard::Up: animable->input.up = false; break;
                    // case sf::Keyboard::Down: animable->input.down = false; break;

                    default: break;
                }
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // auto localdt = deltaClock.getElapsedTime().asSeconds();

        // Hanlde Inputs in the Animable object
        animable->handleInput();

        // Update the states and animation associated to the current movement or action
        animable->updateStates(dt);

        // Clear the window
        window.clear(sf::Color::White);

        // Calculate frame time and FPS
        text.setString(std::format("{:.1f} f/s", 1/dt));
        window.draw(text);

        // Draw the sprite
        window.draw(animable->getSprite());

        // Display what has been drawn
        window.display();

        // Sleep to control frame rate (for demonstration)
        // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Cap at ~60 FPS
    }

    return 0;
}