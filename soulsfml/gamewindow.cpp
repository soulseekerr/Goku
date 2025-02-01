
#include "gamewindow.h"

using namespace soul;

void GameWindow::initialise(const std::string& title, int p_width, int p_height, bool p_fullscreen, int p_frameRateLimit) {

    LoggerManager& logManager = LoggerManager::getInstance();

    _fullscreen = p_fullscreen;

    // Display the list of all the video modes available for fullscreen
    auto modes = sf::VideoMode::getFullscreenModes();

    // for (std::size_t i = 0; i < modes.size(); ++i)
    for (auto & m : modes)
        logManager.log("Mode: {}x{} - {} bpp", m.width, m.height, m.bitsPerPixel);
    
    // sf::ContextSettings contextSettings;
    // contextSettings.depthBits = 24;  // 24-bit depth buffer
    // contextSettings.stencilBits = 8; // 8-bit stencil buffer
    // contextSettings.antialiasingLevel = 4; // 4x anti-aliasing
    // contextSettings.majorVersion = 3; // Request OpenGL 4.x
    // contextSettings.minorVersion = 1;

    // Print context settings
    // std::cout << "Depth bits: " << contextSettings.depthBits << "\n"
    //       << "Stencil bits: " << contextSettings.stencilBits << "\n"
    //       << "Antialiasing level: " << contextSettings.antialiasingLevel << "\n"
    //       << "OpenGL version: " << contextSettings.majorVersion << "." << contextSettings.minorVersion << "\n";

    if (_fullscreen) {
        window.create(
            sf::VideoMode::getFullscreenModes()[0], 
            title,
            sf::Style::Fullscreen);
            // contextSettings);
    } else {
        window.create(
            sf::VideoMode(p_width, p_height), 
            title,
            sf::Style::Default);
            // contextSettings);
    }

    // window.setVerticalSyncEnabled(true); // Enable VSync if needed

    window.setFramerateLimit(p_frameRateLimit);

    if (!ImGui::SFML::Init(window))
        throw GameException("Error while initialising ImGUI!");
}
