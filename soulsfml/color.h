#pragma once

#include <SFML/Graphics/Color.hpp>

namespace soul {

class Color {
private:
    // SFML Color
    sf::Color _color;

public:
    uint8_t r, g, b, a;

public:
    Color() : _color(sf::Color::White) {}
    Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255) 
        : _color(r, g, b, a), r(r), g(g), b(b), a(a) {}

    ~Color() {}

    const sf::Color& get() { 
        _color.r = r;
        _color.g = g;
        _color.b = b;
        _color.a = a;
        return _color; 
    }
};

} // namespace soul