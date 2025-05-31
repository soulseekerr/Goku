
#include "assetmanager.h"
#include "color.h"

using namespace soul;

std::shared_ptr<Texture2d>& AssetManager::getTexture(const std::string& name) {
    std::map<std::string, std::shared_ptr<Texture2d>>::iterator it = mapTextures.find(name);
    if (it == mapTextures.end())
        throw std::invalid_argument("Could not find texture with name " + name);
    return it->second;
}

std::shared_ptr<Texture2d>& AssetManager::addTexture(const std::string& name, const std::string& file_path, bool is_smooth) {
    // Texture exists
    auto& logManager = LoggerManager::getInstance();

    logManager.log("AddTexture {}", name);

    if (textureKeys.search(name)) {
        logManager.log("Texture with name {} was found", name);
        return getTexture(name);
    }

    // Create texture in manager
    logManager.log("Create New Texture with name {}", name);
    auto newTexture = std::make_shared<Texture2d>(name, file_path, is_smooth);

    auto resp = newTexture->loadTexture(file_path);
    if (resp.status == Status::ERROR) throw TextureException(resp.msg.c_str());

    mapTextures[name] = newTexture;
    textureKeys.insert(name);

    return getTexture(name);
}

std::shared_ptr<Texture2d>& AssetManager::addTextureImageFilter(const std::string& name, const std::string& file_path, bool is_smooth, const soul::Color& backgroundColor) {
    // Texture exists
    auto& logManager = LoggerManager::getInstance();

    logManager.log("AddTexture {}", name);

    if (textureKeys.search(name)) {
        logManager.log("Texture with name {} was found", name);
        return getTexture(name);
    }

    // Create texture in manager
    logManager.log("Create New Texture from Image with name {}", name);
    auto newTexture = std::make_shared<Texture2d>(name, file_path, is_smooth);

    auto resp = newTexture->loadTextureFromImage(file_path, backgroundColor);
    if (resp.status == Status::ERROR) throw TextureException(resp.msg.c_str());

    mapTextures[name] = newTexture;
    textureKeys.insert(name);

    return getTexture(name);
}

size_t AssetManager::getCountTextures() const { return textureKeys.size(); }
