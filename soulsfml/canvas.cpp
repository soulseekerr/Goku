
#include "canvas.h"
#include "logger.h"
#include "sprite2d.h"
#include "player.h"
#include "scene.h"

#include <format>
#include <algorithm> // For std::min
#include <random> // For generating random shake offset

using namespace soul;

Canvas::Canvas(const std::string& name, Scene& scene) 
    : Entity(name), _scene(scene) {
}

void Canvas::loadTexts(const std::string& fontFilename) {
    auto& logManager = LoggerManager::getInstance();
    logManager.log("Load Font file {}", fontFilename);

    _font = std::make_shared<sf::Font>();
    if (!_font->openFromFile(fontFilename))
        throw GameException("Could not load font file");
    
    logManager.log("Font file is loaded.");

    _text = std::make_shared<sf::Text>(*_font.get(), toString<size_t>(frameCount), 20);
    _text->setFillColor(sf::Color(240, 255, 255));
    // text.setFillColor(sf::Color(20, 25, 25));
    // text.setCharacterSize(20);

    auto wSize = _scene.gw.window.getSize();
    logManager.log("Window Size: {},{}", wSize.x, wSize.y);

    auto text_y = wSize.y/20 - (float)_text->getCharacterSize();

    _text->setPosition(sf::Vector2f(wSize.x - 150, text_y));

    _lifeText = std::make_shared<sf::Text>(*_font.get(), std::format("Life {}/{}", 
        _scene.player->getAttributes().currentHealth, _scene.player->getAttributes().maxHealth), 20);
    
    _lifeText->setFillColor(sf::Color(155, 155, 255));
    _lifeText->setPosition(sf::Vector2f(wSize.x - 150, text_y + 30));
}

void Canvas::loadBackground(const std::string& bgFilename) {
    auto& logManager = LoggerManager::getInstance();
    // Load Sprite Background
    logManager.log("Loading Texture: {}", bgFilename);
    _background = make_shared<soul::Sprite2d>("Background");
    
    try {
        _background->loadTexture(bgFilename, Vector2f(1.5f, 1.5f), true);
    }
    catch(TextureException& e) {
        logManager.logError("Error occurred while loading Texture: {}\n", e.what());
        exit(-1);
    }
}

// Draw the health bar on the window
// void Canvas::drawHealthBar() {
//     sf::Vector2f barPosition {20.0f, 40.0f};
//     float barWidth = 300.0f;
//     float barHeight = 20.0f;

//     auto maxHealth = scene.player->attributes.maxHealth;
//     auto currentHealth = scene.player->attributes.currentHealth;

//     float healthPercentage = ( (float) currentHealth ) / ( (float) maxHealth );

//     // Set up the background bar (gray)
//     sf::RectangleShape backgroundBar(sf::Vector2f(barWidth, barHeight));
//     backgroundBar.setPosition(barPosition);
//     backgroundBar.setFillColor(sf::Color(50, 50, 50));

//     // Set up the foreground bar (green)
//     sf::RectangleShape healthBar(sf::Vector2f(barWidth * healthPercentage, barHeight));
//     healthBar.setPosition(barPosition);
//     healthBar.setFillColor(sf::Color(100, 250, 50)); // Green color

//     scene.gameWindow.window.draw(backgroundBar);
//     scene.gameWindow.window.draw(healthBar);
// }

// void Canvas::drawFireBar(float dt) {
//     auto& attrs = scene.player->attributes;
//     // auto& input = scene.player->states->input;

//     sf::Vector2f barPosition {20.0f, 70.0f};
//     float barWidth = attrs.maxHealth;
//     float barHeight = 20.0f;

//     // Set up the background bar (gray)
//     sf::RectangleShape backgroundBar(sf::Vector2f(barWidth, barHeight));
//     backgroundBar.setPosition(barPosition);
//     backgroundBar.setFillColor(sf::Color(90, 50, 50));

//     // Set up the foreground bar (green)
//     sf::RectangleShape healthBar(sf::Vector2f(attrs.fireLevel, barHeight));
//     healthBar.setPosition(barPosition);
    
//     scene.gameWindow.window.draw(backgroundBar);
//     scene.gameWindow.window.draw(healthBar);
// }

// Screen shake function
// void Canvas::triggerScreenShake() {
//     auto& attrs = scene.player->attributes;

//     if (attrs.shakeDuration > 0.0f) {
//         // Create random offset
//         float offsetX = static_cast<float>(rand() % 3 - 1) * attrs.shakeIntensity;
//         float offsetY = static_cast<float>(rand() % 3 - 1) * attrs.shakeIntensity;

//         // Apply offset to the view
//         auto currentView = scene.gameWindow.window.getView();
//         currentView.move(offsetX, offsetY);
//         scene.gameWindow.window.setView(currentView);

//         // Decrease shake duration over time
//         attrs.shakeDuration -= 0.02f; // Adjust based on frame timing
//     } else {
//         // Reset view once shake is done
//         scene.gameWindow.window.setView(scene.gameWindow.window.getDefaultView());
//         attrs.shakeDuration = 0.0f;
//     }
// }

bool Canvas::update(float dt) {

        // float MS_PER_FRAME = 1000.0f / 60.0f;
    // float start = getCurrentTimeMS();

    // end loop
    // float elapsed  = getCurrentTimeMS() = start;
    // sleep(max(0, MS_PER_FRAME - elapsed));

    // Calculate frame time and FPS
    // auto frameTime = dt.restart();
    // totalTime += frameTime;
    // frameCount++;

    // _text->setString(std::format("{:.1f} F/S", frameCount / totalTime.asSeconds()));

    // Calculate frame time and FPS
    _text->setString(std::format("{:.1f} f/s", 1/dt));

    auto cur_health = _scene.player->getAttributes().currentHealth;
    auto max_health = _scene.player->getAttributes().maxHealth;

    _lifeText->setString(std::format("Life {}/{}", cur_health, max_health));

    return this->isActive();
}

void Canvas::render() {
    if (_background.get() != nullptr) _scene.gw.window.draw(_background->getSprite());

    if (_text.get() != nullptr) {
        _scene.gw.window.draw(*_text.get());
        _scene.gw.window.draw(*_lifeText.get());
    }

    // drawHealthBar();
    // drawFireBar(totalTime.asSeconds());
}
