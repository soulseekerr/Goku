
#include "texture2d.h"

using namespace soul;


Texture2d::Texture2d(const std::string& name, const std::string& pathFile, bool isSmooth) 
    : Entity(name), _pathFile(pathFile), _isSmooth(isSmooth) {}

Texture2d::~Texture2d() {}

Response Texture2d::loadTexture(const std::string& p_pathfile) {
    const fs::path fsPathFile {p_pathfile};

    if (!fs::exists(fsPathFile)) 
        return Response(Status::ERROR, std::format("Could not find texture file {}", p_pathfile));

    if (!texture.loadFromFile(p_pathfile.c_str()))
        return Response(Status::ERROR, std::format("Error while loading file {} for the texture", p_pathfile));

    texture.setSmooth(_isSmooth);

    return Response(Status::OK, "Texture is loaded");
}

// Helper function to check if two colors are "close enough" with a tolerance
bool Texture2d::colorsAreClose(const sf::Color& color1, const sf::Color& color2, int tolerance) {
    return std::abs(color1.r - color2.r) <= tolerance &&
        std::abs(color1.g - color2.g) <= tolerance &&
        std::abs(color1.b - color2.b) <= tolerance;
}

void Texture2d::makeTransparent(sf::Image &image, sf::Color backgroundColor, int tolerance) {
    sf::Vector2u imageSize = image.getSize();
    for (unsigned int x = 0; x < imageSize.x; ++x) {
        for (unsigned int y = 0; y < imageSize.y; ++y) {
            if (image.getPixel(x, y) == backgroundColor) {
                // std::cout << "set to transparent ()" << x << "," << y << ") " << endl;
                if (colorsAreClose(image.getPixel(x, y), backgroundColor, tolerance)) {
                    image.setPixel(x, y, sf::Color(0, 0, 0, 0));  // Set to transparent
                }
            }
        }
    }
}

Response Texture2d::loadTextureFromImage(const string& p_pathfile, sf::Color backgroundColor) {
    const fs::path fsPathFile {p_pathfile};

    if (!fs::exists(fsPathFile)) 
        return Response(Status::ERROR, std::format("Could not find texture file {}", p_pathfile));
        
    sf::Image image;
    if (!image.loadFromFile(p_pathfile.c_str())) {
        return Response(Status::ERROR, std::format("Error while loading image {} for the texture", p_pathfile));
    }

    // Assuming the purple color is RGB(128, 0, 128) => sf::Color(128, 0, 128)
    std::cout << "make transparent " << endl;
    makeTransparent(image, backgroundColor, 5);

    if (!texture.loadFromImage(image))
        return Response(Status::ERROR, std::format("Error while loading file from image {} for the texture", p_pathfile));

    texture.setSmooth(_isSmooth);

    return Response(Status::OK, "Texture is loaded");
}