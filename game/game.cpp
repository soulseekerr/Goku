
#include "game.h"
#include "player.h"
#include "scene.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

Game::Game() 
    : gw_(soul::GameWindow::getInstance()), 
    player_(std::make_shared<soul::Player>("PlayerSprite")) {

#ifdef DEBUGMODE
    auto& logManager = soul::LoggerManager::getInstance();
    // GUI for monitoring player states
    guiDebugLog_ = make_shared<soul::GuiDebugLog>();
    // Add Log ImGui window to show the logs
    logManager.addLogger(make_shared<soul::LoggerGui>(soul::LOG_LEVEL::LOG_DEBUG, guiDebugLog));
#endif
    gw_.initialise("Goku", 2056, 1329, true, 60);

    // Load data from json file
    player_->loadData();

#ifdef DEBUGMODE
    const std::string spriteTestFile = pathManager.getFilePath(PathManager::FileType::Test, "SpriteTest.png")->string();
    guiStates_ = std::make_shared<soul::GuiAnimableStates>(player_);
    guiSpriteTest_ = std::make_shared<soul::GuiSpriteTest>(spriteTestFile);
#endif

    scene_ = std::make_unique<soul::Scene>(player_);
    scene_->initialise();
}

Game::~Game() {}

int Game::run() {
    sf::Clock deltaClock;
    auto lastTime = std::chrono::high_resolution_clock::now();

    // Main game loop
    while (gw_.window.isOpen()) {

        while (const std::optional event = gw_.window.pollEvent()) {

            ImGui::SFML::ProcessEvent(gw_.window, *event);

            if (event->is<sf::Event::Closed>()) {
                gw_.window.close();
            }

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

                switch (keyPressed->scancode) {

                    case sf::Keyboard::Scancode::Escape:  
                        std::cout << "Exiting the game." << std::endl;
                        gw_.window.close();
                        exit(0);
                    break;

                    case sf::Keyboard::Scancode::Left:  player_->input.left = true; break;
                    case sf::Keyboard::Scancode::Right: player_->input.right = true; break;
                    case sf::Keyboard::Scancode::Up: player_->input.up = true; break;
                    case sf::Keyboard::Scancode::Down: player_->input.down = true; break;
                    case sf::Keyboard::Scancode::P: player_->input.punch = true; break;
                    case sf::Keyboard::Scancode::S: player_->input.shoot = true; break;
                    case sf::Keyboard::Scancode::Space: player_->input.shoot = true; break;
                    case sf::Keyboard::Scancode::K: player_->input.kick = true; break;
                    case sf::Keyboard::Scancode::L: player_->input.kick2 = true; break;
                    case sf::Keyboard::Scancode::E: player_->input.jumpkick = true; break;
                    case sf::Keyboard::Scancode::A: player_->input.punchStick = true; break;
                    case sf::Keyboard::Scancode::D: player_->input.defensive = true; break;
                    case sf::Keyboard::Scancode::Q: player_->input.knockedout = true; break;

                    default: break;
                }
            }

            else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {

                switch (keyReleased->scancode) {

                    case sf::Keyboard::Scancode::Left:  player_->input.left = false; break;
                    case sf::Keyboard::Scancode::Right: player_->input.right = false; break;
                    case sf::Keyboard::Scancode::Up: player_->input.up = false; break;
                    case sf::Keyboard::Scancode::Down: player_->input.down = false; break;
                    case sf::Keyboard::Scancode::P: player_->input.punch = false; break;
                    case sf::Keyboard::Scancode::S: player_->input.shoot = false; break;
                    case sf::Keyboard::Scancode::Space: player_->input.shoot = false; break;
                    case sf::Keyboard::Scancode::K: player_->input.kick = false; break;
                    case sf::Keyboard::Scancode::L: player_->input.kick2 = false; break;
                    case sf::Keyboard::Scancode::E: player_->input.jumpkick = false; break;
                    case sf::Keyboard::Scancode::A: player_->input.punchStick = false; break;
                    case sf::Keyboard::Scancode::D: player_->input.defensive = false; break;
                    case sf::Keyboard::Scancode::Q: player_->input.knockedout = false; break;

                    default: break;
                }
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        ImGui::SFML::Update(gw_.window, sf::seconds(dt));

        // auto localdt = deltaClock.getElapsedTime().asSeconds();

        // Hanlde Inputs in the Animable object
        // Update the states and animation associated to the current movement or action
        player_->update(dt);

        scene_->update(dt);

        // Clear the window
        gw_.window.clear(sf::Color(50, 50, 50, 0));

        player_->render();
        scene_->render();

#ifdef DEBUGMODE
        guiDebugLog_->render(gw_);
        guiStates->render(gw_);
        guiSpriteTest->render(gw_);
#endif

        ImGui::SFML::Render(gw_.window);

        // Display what has been drawn
        gw_.window.display();

        // Sleep to control frame rate (for demonstration)
        // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Cap at ~60 FPS
    }

    ImGui::SFML::Shutdown();

    return 0;
}
