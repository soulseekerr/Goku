
#include "game.h"
#include "player.h"
#include "scene.h"
#include "loaders.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Game::Game() 
    : _gw(soul::GameWindow::getInstance()) {
    _gw.initialise(GAME_NAME, 2056, 1329, true, 60);

    std::cout << "-----------------------------------------" << std::endl;
    std::cout << GAME_NAME << " " << GAME_VERSION << std::endl;
    std::cout << GAME_DESCRIPTION << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

#ifdef DEBUGMODE
    auto& logManager = soul::LoggerManager::getInstance();
    // GUI for monitoring player states
    guiDebugLog_ = make_shared<soul::GuiDebugLog>();
    // Add Log ImGui window to show the logs
    logManager.addLogger(make_shared<soul::LoggerGui>(soul::LOG_LEVEL::LOG_DEBUG, guiDebugLog));
#endif

    const std::string configPath = pathManager.getFilePath(soul::PathManager::FileType::Config, _configFile)->string();
    std::ifstream file(configPath);
    json data;
    file >> data;

    if (!data.contains("scene") || !data["scene"].is_object()) {
        throw std::runtime_error("JSON for Config does not contain a valid 'scene' object.");
    }

    const std::string sceneFile = data["scene"]["filePath"].get<std::string>();
    const std::string scenePath = pathManager.getFilePath(soul::PathManager::FileType::Scene, sceneFile)->string();
    _scene = soul::Loader<soul::Scene>::load(scenePath);

#ifdef DEBUGMODE
    const std::string spriteTestFile = pathManager.getFilePath(PathManager::FileType::Test, "SpriteTest.png")->string();
    guiStates_ = std::make_shared<soul::GuiAnimableStates>(player_);
    guiSpriteTest_ = std::make_shared<soul::GuiSpriteTest>(spriteTestFile);
#endif
}

Game::~Game() {}

int Game::run() {
    sf::Clock deltaClock;
    auto lastTime = std::chrono::high_resolution_clock::now();

    // Main game loop
    while (_gw.window.isOpen()) {

        while (const std::optional event = _gw.window.pollEvent()) {

            ImGui::SFML::ProcessEvent(_gw.window, *event);

            if (event->is<sf::Event::Closed>()) {
                _gw.window.close();
            }

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

                switch (keyPressed->scancode) {

                    case sf::Keyboard::Scancode::Escape:  
                        std::cout << "Exiting the game." << std::endl;
                        _gw.window.close();
                        exit(0);
                    break;

                    case sf::Keyboard::Scancode::Left:  _scene->player->input.left = true; break;
                    case sf::Keyboard::Scancode::Right: _scene->player->input.right = true; break;
                    case sf::Keyboard::Scancode::Up: _scene->player->input.up = true; break;
                    case sf::Keyboard::Scancode::Down: _scene->player->input.down = true; break;
                    case sf::Keyboard::Scancode::P: _scene->player->input.punch = true; break;
                    case sf::Keyboard::Scancode::S: _scene->player->input.shoot = true; break;
                    case sf::Keyboard::Scancode::Space: _scene->player->input.shoot = true; break;
                    case sf::Keyboard::Scancode::K: _scene->player->input.kick = true; break;
                    case sf::Keyboard::Scancode::L: _scene->player->input.kick2 = true; break;
                    case sf::Keyboard::Scancode::E: _scene->player->input.jumpkick = true; break;
                    case sf::Keyboard::Scancode::A: _scene->player->input.punchStick = true; break;
                    case sf::Keyboard::Scancode::D: _scene->player->input.defensive = true; break;
                    case sf::Keyboard::Scancode::Q: _scene->player->input.knockedout = true; break;

                    default: break;
                }
            }

            else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {

                switch (keyReleased->scancode) {

                    case sf::Keyboard::Scancode::Left:  _scene->player->input.left = false; break;
                    case sf::Keyboard::Scancode::Right: _scene->player->input.right = false; break;
                    case sf::Keyboard::Scancode::Up: _scene->player->input.up = false; break;
                    case sf::Keyboard::Scancode::Down: _scene->player->input.down = false; break;
                    case sf::Keyboard::Scancode::P: _scene->player->input.punch = false; break;
                    case sf::Keyboard::Scancode::S: _scene->player->input.shoot = false; break;
                    case sf::Keyboard::Scancode::Space: _scene->player->input.shoot = false; break;
                    case sf::Keyboard::Scancode::K: _scene->player->input.kick = false; break;
                    case sf::Keyboard::Scancode::L: _scene->player->input.kick2 = false; break;
                    case sf::Keyboard::Scancode::E: _scene->player->input.jumpkick = false; break;
                    case sf::Keyboard::Scancode::A: _scene->player->input.punchStick = false; break;
                    case sf::Keyboard::Scancode::D: _scene->player->input.defensive = false; break;
                    case sf::Keyboard::Scancode::Q: _scene->player->input.knockedout = false; break;

                    default: break;
                }
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        ImGui::SFML::Update(_gw.window, sf::seconds(dt));

        // auto localdt = deltaClock.getElapsedTime().asSeconds();

        _scene->update(dt);

        // Clear the window
        _gw.window.clear(sf::Color(50, 50, 50, 0));

        _scene->render();

#ifdef DEBUGMODE
        guiDebugLog_->render(gw_);
        guiStates->render(gw_);
        guiSpriteTest->render(gw_);
#endif

        ImGui::SFML::Render(_gw.window);

        // Display what has been drawn
        _gw.window.display();

        // Sleep to control frame rate (for demonstration)
        // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Cap at ~60 FPS
    }

    ImGui::SFML::Shutdown();

    return 0;
}
