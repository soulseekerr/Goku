
#include "texture2d.h"

using namespace soul;


Texture2d::Texture2d(const std::string& name, const std::string& pathFile, bool isSmooth) 
    : Entity(name), _pathFile(pathFile), _isSmooth(isSmooth) {}

Texture2d::~Texture2d() {}

Response Texture2d::loadTexture(const std::string& filePath) {
    const fs::path fsPathFile {filePath};

    if (!fs::exists(fsPathFile)) 
        return Response(Status::ERROR, std::format("Could not find texture file {}", filePath));

    if (!texture.loadFromFile(filePath.c_str()))
        return Response(Status::ERROR, std::format("Error while loading file {} for the texture", filePath));

    texture.setSmooth(_isSmooth);

    return Response(Status::OK, "Texture is loaded");
}

// Helper function to check if two colors are "close enough" with a tolerance
bool Texture2d::colorsAreClose(const soul::Color& color1, const soul::Color& color2, int tolerance) {
    return std::abs(color1.r - color2.r) <= tolerance &&
        std::abs(color1.g - color2.g) <= tolerance &&
        std::abs(color1.b - color2.b) <= tolerance;
}

void Texture2d::makeTransparent(soul::Image& image, const soul::Color& bgColor, int tolerance) {
    sf::Vector2u imageSize = image.getSize();
    for (unsigned int x = 0; x < imageSize.x; ++x) {
        for (unsigned int y = 0; y < imageSize.y; ++y) {
            
            sf::Color color1 = image.getPixel(x, y);
            soul::Color scolor1 = soul::Color(color1.r, color1.g, color1.b, color1.a);

            if (scolor1.r == bgColor.r && scolor1.g == bgColor.g && scolor1.b == bgColor.b) {
                
                // std::cout << "set to transparent ()" << x << "," << y << ") " << endl;
                if (colorsAreClose(scolor1, bgColor, tolerance)) {
        
                    image.setPixel(x, y, sf::Color(0, 0, 0, 0));  // Set to transparent
                }
            }
        }
    }
}

Response Texture2d::loadTextureFromImage(const std::string& filePath, const soul::Color& bgColor) {
    const fs::path fsPathFile {filePath};

    if (!fs::exists(fsPathFile)) 
        return Response(
            Status::ERROR, 
            std::format("Could not find texture file {}", filePath));
        
    soul::Image image;
    auto resp = image.loadFromFile(filePath.c_str());
    if (resp.status == Status::ERROR) {
        return resp;
    }

    // Assuming the purple color is RGB(128, 0, 128) => sf::Color(128, 0, 128)
    std::cout << "make transparent " << std::endl;
    makeTransparent(image, bgColor, 5);

    if (!texture.loadFromImage(image.getImage())) {
        return Response(
            Status::ERROR, 
            "Error while loading file from image for the texture");
    }

    texture.setSmooth(_isSmooth);

    return Response(Status::OK, "Texture is loaded");
}