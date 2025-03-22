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

class Canvas : public Entity {
private:
    // Referemce to the Scene and underlying instances
    Scene& _scene;
    // Background sprite
    std::shared_ptr<Sprite2d> _background;
    // Font for the text
    std::shared_ptr<sf::Font> _font;
    // Text object
    std::shared_ptr<sf::Text> _text;
    // Life text object
    std::shared_ptr<sf::Text> _lifeText;

public:
    // Total time elapsed
    sf::Time totalTime {};
    // Frame count
    size_t frameCount {};

public:
    Canvas() = delete;
    explicit Canvas(const std::string& name, Scene& scene);
    virtual ~Canvas() = default;

    void loadTexts(const std::string& fontFilename);
    void loadBackground(const std::string& bgFilename);
    
    virtual bool update(float dt) override;
    virtual void render() override;

    // void drawHealthBar();
    // void drawFireBar(float dt);
    // void triggerScreenShake();
};

} // namespace soul