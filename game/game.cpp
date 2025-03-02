
#include "game.h"
#include "player.h"

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

    player_->loadData();

#ifdef DEBUGMODE
    guiStates_ = std::make_shared<soul::GuiAnimableStates>(player_);
    const std::string spriteTestFile = "/Users/soulseeker/Projects/GitHub/gokugame/textures/Kid Goku.png";
    guiSpriteTest_ = std::make_shared<soul::GuiSpriteTest>(spriteTestFile);
#endif
}

Game::~Game() {}

int Game::run() {
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
   
    auto text_y = gw_.window.getSize().y/20 - (float)text.getCharacterSize();
    text.setPosition(sf::Vector2f(gw_.window.getSize().x - 150, text_y));

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

        // Clear the window
        gw_.window.clear(sf::Color(50, 50, 50, 0));

        // Calculate frame time and FPS
        text.setString(std::format("{:.1f} f/s", 1/dt));
        gw_.window.draw(text);

        player_->render();

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
