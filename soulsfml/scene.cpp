
#include "scene.h"
#include "logger.h"
#include "player.h"

using namespace soul;

void Scene::initialise() {
    const std::string fontFilename = pathManager.getFilePath(PathManager::FileType::Font, "MontereyFLF-Bold.ttf")->string();
    logManager.log("Font file {}", fontFilename);
    const std::string bgFilename = pathManager.getFilePath(PathManager::FileType::Texture, "background.png")->string();
    logManager.log("Background file {}", bgFilename);

    logManager.log("Initialise canvasLayer in Scene:");
    canvasLayer->loadTexts(fontFilename);
}

void Scene::update(float dt) {
    canvasLayer->update(dt);
}

void Scene::render() {
    canvasLayer->render();
}