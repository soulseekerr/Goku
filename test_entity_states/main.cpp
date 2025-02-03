
#include "core/logger.h"
#include "assetmanager.h"
#include "animable.h"
#include "state.h"
#include "core/utils.h"
#include "gui.h"
#include "gamewindow.h"
#include "entitymanager.h"

#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

void spawnFireball() {
    // Logger access
    auto& logManager = soul::LoggerManager::getInstance();
    // Container for entities
    soul::EntityManager&  entities = soul::EntityManager::getInstance();

    const std::string spriteFilename = "/Users/soulseeker/Projects/GitHub/gokugame/debug/textures/fireballs.png";
    auto n = 1;
    for (auto i = 0; i < n; ++i) {
        // Dont use auto keyword here to allow implicit casting
        std::shared_ptr<soul::Animable> fb = std::make_shared<soul::Animable>("Fireball");

        const soul::sSpriteData spriteDataFireball = {
            spriteFilename, // Filename for the image file
            soul::Vector2f(1.5f, 1.5f), // Scaling the sprite from the image
            soul::Vector2i(10, 128), // Initial px position in the Sprite sheet
            soul::Vector2i(70, 70), // Initial px size in the Sprite sheet
            soul::Vector2i(300, 450), // Coordinates px of screen as they are defined 0,0 from top left.
            false, // Filter Transparency By Color
            soul::Color(0, 0, 0, 0) // Filtering Color
        };

        const soul::sTransformScalars scalarsFireball {
            20.0f, // initialVelocityX
            0.0f,  // initialVelocityY
            0.0f, // gravity
            1, // direction
            410 // groundY
        };

        fb->load(spriteDataFireball, scalarsFireball);

        fb->getSprite().setRotation(-90.0f);

        auto animFire = std::make_shared<soul::SpriteAnimation>();
        animFire->addFrame(0, 0, 70, 70, 0.05f);
        animFire->addFrame(140, 0, 70, 70, 0.05f);
        animFire->addFrame(210, 0, 70, 70, 0.05f);
        animFire->addFrame(280, 0, 70, 70, 0.05f);
        animFire->addFrame(350, 0, 70, 70, 0.05f);
        animFire->addFrame(420, 0, 70, 70, 0.05f);
        animFire->addFrame(490, 0, 70, 70, 0.05f);
        animFire->addFrame(560, 0, 70, 70, 0.05f);
        animFire->addFrame(630, 0, 70, 70, 0.05f);
        animFire->addFrame(700, 0, 70, 70, 0.05f);
        animFire->addFrame(770, 0, 70, 70, 0.05f);
        animFire->addFrame(840, 0, 70, 70, 0.05f);
        animFire->addFrame(910, 0, 70, 70, 0.05f);
        animFire->addFrame(980, 0, 70, 70, 0.05f);
        animFire->addFrame(1050, 0, 70, 70, 0.05f);
        fb->addAnimation(soul::AnimationState::ShootFireball, animFire);

        fb->resetPosition();

        entities.addEntity(fb);

        // 3.0f, // Adjust bullet position to originate from player and set initial lifetime
        // 150.0f,  // initial velocity on X
        logManager.log("Add Fireball {} (x,y)=({},{})", i, fb->getPosition().x, fb->getPosition().y);
    }
}

int main(int argc, char** argv) {
    auto& logManager = soul::LoggerManager::getInstance();
    logManager.addLogger(make_shared<soul::LoggerConsole>(soul::LOG_LEVEL::LOG_DEBUG));
    logManager.log("Starting Sprite Animation Test");

    // Container for entities
    soul::EntityManager&  entities = soul::EntityManager::getInstance();

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

    auto animable = std::make_shared<soul::Animable>("PlayerSprite");

    const std::string filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/Kid Goku.png";

    const soul::sSpriteData spriteData = {
        filePath, // Filename for the image file
        soul::Vector2f(1.2f, 1.2f), // Scaling the sprite from the image
        soul::Vector2i(10, 128), // Initial px position in the Sprite sheet
        soul::Vector2i(80, 102), // Initial px size in the Sprite sheet
        soul::Vector2i(300, 450), // Coordinates px of screen as they are defined 0,0 from top left.
        false, // Filter Transparency By Color
        soul::Color(0, 0, 0, 0) // Filtering Color
    };

    const soul::sTransformScalars scalars = {
        6.0f, // initialVelocityX
        -700.0f,  // initialVelocityY
        1700.0f, // gravity
        1, // direction
        450 // groundY
    };

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
    animPunch->addFrame(420, 1535, 90, 85, 0.10f);
    animPunch->addFrame(510, 1535, 70, 85, 0.10f);
    animPunch->addFrame(580, 1535, 85, 85, 0.10f);
    animable->addAnimation(soul::AnimationState::Punch, animPunch);

    animable->setAnimationState(soul::AnimationState::Idle);
    animable->setMovementState(*stateManager.getMovementState<soul::IdleState>(soul::AnimationState::Idle));
    animable->setActionState(*stateManager.getActionState<soul::ActionIdleState>(soul::AnimationState::ActionIdle));

    animable->resetPosition();

    entities.addEntity(animable);

    std::shared_ptr<soul::GuiAnimableStates> guiStates = std::make_shared<soul::GuiAnimableStates>(animable);
    std::shared_ptr<soul::GuiSpriteTest> guiSpriteTest = std::make_shared<soul::GuiSpriteTest>();

    sf::Clock deltaClock;
    auto lastTime = std::chrono::high_resolution_clock::now();

    sf::Font font;
    sf::Text text;

    const string fontFilename = "/Users/soulseeker/Projects/cpp_projects/sfml_sparky/build/bin/fonts/MontereyFLF-Bold.ttf";
    font.loadFromFile(fontFilename);

    text.setFont(font);
    text.setFillColor(sf::Color(20, 25, 25));
    text.setCharacterSize(20);
   
    auto text_y = gw.window.getSize().y/20 - (float)text.getCharacterSize();
    text.setPosition(gw.window.getSize().x - 150, text_y);

    // Main game loop
    while (gw.window.isOpen()) {
        // Handle events
        sf::Event event;
        while (gw.window.pollEvent(event)) {

            ImGui::SFML::ProcessEvent(gw.window, event);

            if (event.type == sf::Event::Closed) {
                gw.window.close();
            }

            if (event.type == sf::Event::KeyPressed) {

                switch (event.key.code) {

                    case sf::Keyboard::Escape:  
                        cout << "Exiting." << endl;
                        gw.window.close();
                        exit(0);
                    break;
                    
                    case sf::Keyboard::Left:  animable->input.left = true; break;
                    case sf::Keyboard::Right: animable->input.right = true; break;
                    case sf::Keyboard::Up: animable->input.up = true; break;
                    case sf::Keyboard::Down: animable->input.down = true; break;
                    case sf::Keyboard::P: animable->input.punch = true; break;
                    
                    default: break;
                }
            }
            
            if (event.type == sf::Event::KeyReleased) {

                switch (event.key.code) {

                    case sf::Keyboard::Left:  animable->input.left = false; break;
                    case sf::Keyboard::Right: animable->input.right = false; break;
                    case sf::Keyboard::Up: animable->input.up = false; break;
                    case sf::Keyboard::Down: animable->input.down = false; break;
                    case sf::Keyboard::P: animable->input.punch = false; break;

                    default: break;
                }
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        ImGui::SFML::Update(gw.window, sf::seconds(dt));

        // auto localdt = deltaClock.getElapsedTime().asSeconds();

        // important to refresh the entities in the manager instance
        // also to delete from memory any dead entities
        entities.update();

        // Hanlde Inputs in the Animable objects
        for (const auto& entity : entities.getEntities()) {
            entity->handleInput();
            // Update the states and animation associated to the current movement or action
            entity->updateStates(dt);
        }

        // Shoot Fireball
        // Check if the player is shooting, if yes do not allow shooting
        // if (animable->input.shoot && !animable->input.is_shooting) {
        //     animable->input.is_shooting = true; // we lock the shoot until fireball is dead
        //     animable->input.shoot = false;  // applied once in case we keep pressing the button 
        //     spawnFireball();
        // }

        // Clear the window
        gw.window.clear(sf::Color(50, 50, 50, 0));

        // Calculate frame time and FPS
        text.setString(std::format("{:.1f} f/s", 1/dt));
        gw.window.draw(text);

        // Draw all the entities
        for (const auto& e : entities.getEntities()) {
            if (!e->isActive()) continue;

            gw.window.draw(e->getSprite());
        }

        guiDebugLog->render(gw);
        guiStates->render(gw);
        guiSpriteTest->render(gw);

        ImGui::SFML::Render(gw.window);

        // Display what has been drawn
        gw.window.display();

        // Sleep to control frame rate (for demonstration)
        // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Cap at ~60 FPS
    }

    //ImGui::SFML::Shutdown();

    return 0;
}