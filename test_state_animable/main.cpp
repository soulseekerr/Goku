
#include "core/logger.h"
#include "assetmanager.h"
#include "animable.h"
#include "state.h"
#include "core/utils.h"
#include "gui.h"
#include "gamewindow.h"

#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

int main(int argc, char** argv) {
    auto& logManager = soul::LoggerManager::getInstance();
    logManager.addLogger(make_shared<soul::LoggerConsole>(soul::LOG_LEVEL::LOG_DEBUG));
    logManager.log("Starting Sprite Animation Test");

    // GUI for monitoring player states
    std::shared_ptr<soul::GuiDebugLog> guiDebugLog;
    guiDebugLog = make_shared<soul::GuiDebugLog>();

    // Add Log ImGui window to show the logs
    logManager.addLogger(make_shared<soul::LoggerGui>(soul::LOG_LEVEL::LOG_DEBUG, guiDebugLog));

    // Create a window
    soul::GameWindow& gw = soul::GameWindow::getInstance();
    gw.initialise("Goku Game: Animable Object States", 800, 600, false, 60);
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Animable Object States");
    // window.setFramerateLimit(60);

    auto name = "PlayerSprite";
    auto filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/Kid Goku.png";

    soul::StateManager stateManager;
    soul::IdleState* idleState = stateManager.registerMovement<soul::IdleState>(soul::AnimationState::Idle);
    soul::JumpState* jumpState = stateManager.registerMovement<soul::JumpState>(soul::AnimationState::Jump);
    soul::WalkState* walkState = stateManager.registerMovement<soul::WalkState>(soul::AnimationState::Walk);
    soul::ActionIdleState* actionIdleState = stateManager.registerAction<soul::ActionIdleState>(soul::AnimationState::ActionIdle);
    soul::PunchState* punchState = stateManager.registerAction<soul::PunchState>(soul::AnimationState::Punch);

    idleState->defineDependencies(stateManager);
    jumpState->defineDependencies(stateManager);
    walkState->defineDependencies(stateManager);
    actionIdleState->defineDependencies(stateManager);
    punchState->defineDependencies(stateManager);

    auto animable = std::make_shared<soul::Animable>(name);

    soul::sSpriteData spriteData = {
        filePath, // Filename for the image file
        soul::Vector2f(1.2f, 1.2f), // Scaling the sprite from the image
        soul::Vector2i(10, 128), // Initial px position in the Sprite sheet
        soul::Vector2i(80, 102), // Initial px size in the Sprite sheet
        soul::Vector2i(300, 450), // Coordinates px of screen as they are defined 0,0 from top left.
        false, // Filter Transparency By Color
        soul::Color(0, 0, 0, 0) // Filtering Color
    };

    soul::sTransformScalars scalars;
    scalars.initialVelocityX = 6.0f;
    scalars.gravity = 1700.0f;
    scalars.initialVelocityY = -700.0f;
    scalars.groundY = 450;

    animable->load(spriteData, scalars);
    
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
    auto animJump = std::make_shared<soul::SpriteAnimation>();
    animJump->addFrame(190, 550, 80, 115, 0.10f);
    animJump->addFrame(190, 550, 80, 115, 0.10f);
    animJump->addFrame(190, 550, 80, 115, 0.10f);
    animJump->addFrame(190, 550, 80, 115, 0.10f);
    animJump->addFrame(460, 550, 80, 115, 0.20f);
    animJump->addFrame(460, 550, 80, 115, 0.10f);
    animJump->addFrame(10, 128, 80, 102, 0.20f);
    animable->addAnimation(soul::AnimationState::Jump, animJump);

    auto animPunch = std::make_shared<soul::SpriteAnimation>();
    animPunch->addFrame(420, 1535, 90, 83, 0.10f);
    animPunch->addFrame(510, 1535, 70, 83, 0.10f);
    animPunch->addFrame(580, 1535, 85, 83, 0.10f);
    animable->addAnimation(soul::AnimationState::Punch, animPunch);

    animable->setAnimationState(soul::AnimationState::Idle);
    animable->setMovementState(*stateManager.getMovementState<soul::IdleState>(soul::AnimationState::Idle));
    animable->setActionState(*stateManager.getActionState<soul::ActionIdleState>(soul::AnimationState::ActionIdle));

    animable->resetPosition();

    std::shared_ptr<soul::GuiAnimableStates> guiStates = std::make_shared<soul::GuiAnimableStates>(animable);
    const std::string spriteTestFile = "/Users/soulseeker/Projects/GitHub/gokugame/textures/Kid Goku.png";
    std::shared_ptr<soul::GuiSpriteTest> guiSpriteTest = std::make_shared<soul::GuiSpriteTest>(spriteTestFile);

    sf::Clock deltaClock;
    auto lastTime = std::chrono::high_resolution_clock::now();

    sf::Font font;
    const string fontFilename = "/Users/soulseeker/Projects/cpp_projects/sfml_sparky/build/bin/fonts/MontereyFLF-Bold.ttf";
    if (!font.openFromFile(fontFilename)) {
        std::cout << "Error while opening font file!" << fontFilename << std::endl;
        return -1;
    }

    sf::Text text(font);
    text.setFillColor(sf::Color(20, 25, 25));
    text.setCharacterSize(20);
   
    auto text_y = gw.window.getSize().y/20 - (float)text.getCharacterSize();
    text.setPosition(sf::Vector2f(gw.window.getSize().x - 150, text_y));

    // Main game loop
    while (gw.window.isOpen()) {

        while (const std::optional event = gw.window.pollEvent()) {

            ImGui::SFML::ProcessEvent(gw.window, *event);

            if (event->is<sf::Event::Closed>()) {
                gw.window.close();
            }

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

                switch (keyPressed->scancode) {

                    case sf::Keyboard::Scancode::Escape:  
                        std::cout << "Exiting the game." << std::endl;
                        gw.window.close();
                        exit(0);
                    break;

                    case sf::Keyboard::Scancode::Left:  animable->input.left = true; break;
                    case sf::Keyboard::Scancode::Right: animable->input.right = true; break;
                    case sf::Keyboard::Scancode::Up: animable->input.up = true; break;
                    case sf::Keyboard::Scancode::Down: animable->input.down = true; break;
                    case sf::Keyboard::Scancode::P: animable->input.punch = true; break;

                    default: break;
                }
            }

            else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {

                switch (keyReleased->scancode) {

                    case sf::Keyboard::Scancode::Left:  animable->input.left = false; break;
                    case sf::Keyboard::Scancode::Right: animable->input.right = false; break;
                    case sf::Keyboard::Scancode::Up: animable->input.up = false; break;
                    case sf::Keyboard::Scancode::Down: animable->input.down = false; break;
                    case sf::Keyboard::Scancode::P: animable->input.punch = false; break;

                    default: break;
                }
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        ImGui::SFML::Update(gw.window, sf::seconds(dt));

        // auto localdt = deltaClock.getElapsedTime().asSeconds();

        // Hanlde Inputs in the Animable object
        // Update the states and animation associated to the current movement or action
        animable->update(dt);

        // Clear the window
        gw.window.clear(sf::Color(50, 50, 50, 0));

        // Calculate frame time and FPS
        text.setString(std::format("{:.1f} f/s", 1/dt));
        gw.window.draw(text);

        gw.window.draw(animable->getSprite());

        guiDebugLog->render(gw);
        guiStates->render(gw);
        guiSpriteTest->render(gw);

        ImGui::SFML::Render(gw.window);

        // Display what has been drawn
        gw.window.display();

        // Sleep to control frame rate (for demonstration)
        // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Cap at ~60 FPS
    }

    ImGui::SFML::Shutdown();

    return 0;
}