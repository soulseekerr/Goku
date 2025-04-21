#pragma once

#include "entity.h"
#include "sprite2d.h"
#include "gamewindow.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>

namespace soul {

class Scene;
class Sprite2d;

struct CanvasProperties {
    // Global
    std::string fontFilename;
    int fontSize;
    sf::Color fontColor;
    // Health Bar
    sf::Vector2f hbarPosition {};
    float hbarWidth;
    float hbarHeight;
    sf::Color hbbgColor;
    sf::Color hbfgColor;
};

class Canvas : public Entity {
private:
    // Referemce to the Scene and underlying instances
    Scene& _scene;
    // Props
    CanvasProperties _props;
    // Background sprite
    std::shared_ptr<Sprite2d> _background;
    // Font for the text
    std::shared_ptr<sf::Font> _font;
    // Text object
    std::shared_ptr<sf::Text> _text;
    // Life text object
    std::shared_ptr<sf::Text> _lifeText;
    // Health bar
    std::shared_ptr<sf::RectangleShape> _backgroundBar;
    std::shared_ptr<sf::RectangleShape> _foregroundBar;

public:
    // Total time elapsed
    sf::Time totalTime {};
    // Frame count
    size_t frameCount {};

public:
    Canvas() = delete;
    explicit Canvas(const std::string& name, Scene& scene);
    virtual ~Canvas() = default;

    void load(const CanvasProperties& props);

    void loadBackground(const std::string& bgFilename);
    
    virtual bool update(float dt) override;
    virtual void render() override;

    void drawHealthBar();
    // void drawFireBar(float dt);
    // void triggerScreenShake();
};

} // namespace soul