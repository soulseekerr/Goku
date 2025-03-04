#pragma once

#include "utils.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace soul {

class Image {
private:
    sf::Image _image;

public:
    Image() {}
    ~Image() {}

    sf::Image& getImage() {
        return _image;
    }

    Response loadFromFile(const std::string& pathFile) {
        if (!_image.loadFromFile(pathFile.c_str())) {
            return Response(Status::ERROR, "Error while loading image for the texture");
        }
        return Response(Status::OK, "Image is loaded");
    }

    const sf::Vector2u getSize() const {
        return _image.getSize();
    }

    sf::Color getPixel(unsigned int x, unsigned int y) { 
        return _image.getPixel(sf::Vector2u(x, y));
    }

    void setPixel(unsigned int x, unsigned int y, sf::Color color) { 
        _image.setPixel(sf::Vector2u(x, y), color);
    }
};

} // namespace soul