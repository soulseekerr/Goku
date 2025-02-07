
#include "core/logger.h"
#include "assetmanager.h"
#include "player.h"
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

int main(int argc, char** argv) {
    auto& logManager = soul::LoggerManager::getInstance();
    logManager.addLogger(make_shared<soul::LoggerConsole>(soul::LOG_LEVEL::LOG_DEBUG));
    logManager.log("Starting Sprite Animation Test");

    // Container for entities
    soul::EntityManager&  entities = soul::EntityManager::getInstance();

    // GUI for monitoring player states
    std::shared_ptr<soul::GuiDebugLog> guiDebugLog = std::make_shared<soul::GuiDebugLog>();

    // Add Log ImGui window to show the logs
    logManager.addLogger(make_shared<soul::LoggerGui>(soul::LOG_LEVEL::LOG_DEBUG, guiDebugLog));

    // Create a window
    soul::GameWindow& gw = soul::GameWindow::getInstance();
    gw.initialise("Goku Game: Animable Object States", 800, 600, false, 60);
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Animable Object States");
    // window.setFramerateLimit(60);

    std::shared_ptr<soul::Animable> player = std::make_shared<soul::Player>("PlayerSprite");
    player->loadData();
    
    std::shared_ptr<soul::GuiAnimableStates> guiStates = std::make_shared<soul::GuiAnimableStates>(player);
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

                    case sf::Keyboard::Scancode::Left:  player->input.left = true; break;
                    case sf::Keyboard::Scancode::Right: player->input.right = true; break;
                    case sf::Keyboard::Scancode::Up: player->input.up = true; break;
                    case sf::Keyboard::Scancode::Down: player->input.down = true; break;
                    case sf::Keyboard::Scancode::P: player->input.punch = true; break;
                    case sf::Keyboard::Scancode::S: player->input.shoot = true; break;
                    case sf::Keyboard::Scancode::Space: player->input.shoot = true; break;

                    default: break;
                }
            }

            else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {

                switch (keyReleased->scancode) {

                    case sf::Keyboard::Scancode::Left:  player->input.left = false; break;
                    case sf::Keyboard::Scancode::Right: player->input.right = false; break;
                    case sf::Keyboard::Scancode::Up: player->input.up = false; break;
                    case sf::Keyboard::Scancode::Down: player->input.down = false; break;
                    case sf::Keyboard::Scancode::P: player->input.punch = false; break;
                    case sf::Keyboard::Scancode::S: player->input.shoot = false; break;
                    case sf::Keyboard::Scancode::Space: player->input.shoot = false; break;

                    default: break;
                }
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        ImGui::SFML::Update(gw.window, sf::seconds(dt));

        // Update the player 
        player->update(dt);

        // auto localdt = deltaClock.getElapsedTime().asSeconds();

        // important to refresh the entities in the manager instance
        // also to delete from memory any dead entities
        entities.update();

        // Hanlde Inputs in the Animable objects
        for (const auto& entity : entities.getEntities()) {
            // Update the states and animation associated to the current movement or action
            entity->update(dt);
        }

        // Clear the window
        gw.window.clear(sf::Color(50, 50, 50, 0));

        // Calculate frame time and FPS
        text.setString(std::format("{:.1f} f/s", 1/dt));
        gw.window.draw(text);

        // Draw all the entities
        for (const auto& e : entities.getEntities()) {
            if (!e->isActive()) {

                logManager.log("Entity is no longer active {}", e->tag());
                continue;
            }

            e->render();
        }

        gw.window.draw(player->getSprite());

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