#pragma once

#include "player.h"
#include "canvas.h"
#include "scene.h"
#include "tile.h"

#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace soul {

/**
 * @brief Get Animation State from String
 */
static soul::AnimationState animationStateFromString(const std::string& str) {
    static const std::unordered_map<std::string, soul::AnimationState> s_map = {
        {"Idle", soul::AnimationState::Idle},
        {"Walk", soul::AnimationState::Walk},
        {"Jump", soul::AnimationState::Jump},
        {"Punch", soul::AnimationState::Punch},
        {"PunchStick", soul::AnimationState::PunchStick},
        {"Shoot", soul::AnimationState::Shoot},
        {"Kick", soul::AnimationState::Kick},
        {"Kick2", soul::AnimationState::Kick2},
        {"JumpKick", soul::AnimationState::JumpKick},
        {"Defensive", soul::AnimationState::Defensive},
        {"Knocked", soul::AnimationState::Knocked},
        {"ShootFireball", soul::AnimationState::ShootFireball}
    };

    auto it = s_map.find(str);
    if (it != s_map.end()) return it->second;
    throw std::runtime_error("Unknown AnimationState: " + str);
}

/**
 * @brief Loader class
 * Class that loads data from a JSON file and creates an instance of the specified type.
 */
template<typename T>
struct Loader {
    static std::unique_ptr<T> load(std::string_view jsonPath) {
        static_assert(sizeof(T) == 0, "No loader defined for this type.");
        // This will cause a compile-time error if the specialization is not provided
        return nullptr;
    }
};

/**
 * @brief SceneLoader class
 * Class that loads data from a JSON file and creates an instance of the specified type.
 * This is a specialization which adds a Scene reference in argument.
 */
template<typename T>
struct SceneLoader {
    static std::unique_ptr<T> load(std::string_view jsonPath, Scene& scene) {
        static_assert(sizeof(T) == 0, "No scene loader defined for this type.");
        // This will cause a compile-time error if the specialization is not provided
        return nullptr;
    }
};

/**
 * @brief Loader specialization for Player
 * Loads Player data from a JSON file and creates an instance of Player.
 * No Scene reference is needed in argument.
 */
template<>
struct Loader<Player> {
    static std::unique_ptr<Player> load(std::string_view jsonPath) {
        LoggerManager& logManager = LoggerManager::getInstance();

        std::filesystem::path fullPath = jsonPath;
        std::string filename = fullPath.filename().string();
        if (!fullPath.has_extension()) {
            throw std::runtime_error(std::format("JSON file {} does not have a valid extension.", filename));
        }
        if (fullPath.extension() != ".json") {
            throw std::runtime_error(std::format("JSON file {} does not have a valid JSON extension.", filename));
        }
    
        logManager.log("Loading Player data from: ", jsonPath.data());

        std::ifstream file(jsonPath.data());
        json data;
        file >> data;

        if (!data.contains("attributes") || !data["attributes"].is_object()) {
            throw std::runtime_error("JSON for Player does not contain a valid 'attributes' object.");
        }

        if (!data.contains("spriteData") || !data["spriteData"].is_object()) {
            throw std::runtime_error("JSON for Player does not contain a valid 'spriteData' object.");
        }

        if (!data.contains("animations") || !data["animations"].is_object()) {
            throw std::runtime_error("JSON for Player does not contain a valid 'animations' object.");
        }
    
        if (!data.contains("transformScalars") || !data["transformScalars"].is_object()) {
            throw std::runtime_error("JSON for Player does not contain a valid 'transformScalars' object.");
        }

        // Load the attributes of the player
        PlayerAttributes attributes;

        attributes.currentHealth = data["attributes"]["current_health"];
        attributes.maxHealth = data["attributes"]["max_health"];
        attributes.currentMana = data["attributes"]["current_mana"];
        attributes.maxMana = data["attributes"]["max_mana"];
        attributes.currentExp = data["attributes"]["current_exp"];
        attributes.maxExp = data["attributes"]["max_exp"];
        attributes.level = data["attributes"]["level"];

        const soul::sSpriteData spriteData = {
            // Filename for the image file
            data["spriteData"]["spritePath"],
            // Scaling the sprite from the image
            soul::Vector2f(data["spriteData"]["scale"][0], data["spriteData"]["scale"][1]),
            // Initial px position in the Sprite sheet
            soul::Vector2i(data["spriteData"]["spriteSheetPosition"][0], data["spriteData"]["spriteSheetPosition"][1]),
            // Initial px size in the Sprite sheet
            soul::Vector2i(data["spriteData"]["spriteSheetSize"][0], data["spriteData"]["spriteSheetSize"][1]),
            // Coordinates px of screen as they are defined 0,0 from top left.
            soul::Vector2f(data["spriteData"]["screenPosition"][0], data["spriteData"]["screenPosition"][1]), 
            // Filter Transparency By Color
            data["spriteData"]["filterTransparency"], 
            // Filtering Color
            soul::Color(
                data["spriteData"]["filterColor"][0],
                data["spriteData"]["filterColor"][1],
                data["spriteData"]["filterColor"][2],
                data["spriteData"]["filterColor"][3]) 
        };
    
        const soul::sTransformScalars scalars = {
            data["transformScalars"]["initialVelocityX"],
            data["transformScalars"]["initialVelocityY"],
            data["transformScalars"]["gravity"],
            data["transformScalars"]["direction"],
            data["transformScalars"]["angle"],
            data["transformScalars"]["groundY"]
        };
    
        auto inst = std::make_unique<Player>("PlayerSprite");
        logManager.log("Class {} instance", __TYPE_NAME__);

        inst->setAttributes(std::move(attributes));
        inst->load(spriteData, scalars);
        inst->initFireballs(
            data["fireballs"]["jsonPath"].get<std::string>(), // load the json file for fireball data
            data["fireballs"]["speedX"].get<float>(),
            data["fireballs"]["lifetime"].get<float>());

        logManager.log("Loading Animation data");

        for (auto& [animationName, animationInfo] : data["animations"].items()) {

            auto animationState = animationStateFromString(animationName);
            std::string type = animationInfo["type"];

            std::cout << "Animation: " << animationName << " (type: " << type << ")\n";
            auto spriteAnim = std::make_shared<soul::SpriteAnimation>();
            for (const auto& f : animationInfo["frames"]) {
                spriteAnim->addFrame(f["x"], f["y"], f["width"], f["height"], f["duration"]);
            }
            inst->addAnimation(animationState, spriteAnim);
        }

        inst->resetPosition();

        soul::IdleState* idleState = inst->registerMovement<soul::IdleState>(soul::AnimationState::Idle);
        soul::JumpState* jumpState = inst->registerMovement<soul::JumpState>(soul::AnimationState::Jump);
        soul::WalkState* walkState = inst->registerMovement<soul::WalkState>(soul::AnimationState::Walk);
        soul::ActionIdleState* actionIdleState = inst->registerAction<soul::ActionIdleState>(soul::AnimationState::ActionIdle);
        soul::PunchState* punchState = inst->registerAction<soul::PunchState>(soul::AnimationState::Punch);
        soul::PunchStickState* punchStickState = inst->registerAction<soul::PunchStickState>(soul::AnimationState::PunchStick);
        soul::ShootState* shootState = inst->registerAction<soul::ShootState>(soul::AnimationState::ShootFireball);
        soul::KickState* kickState = inst->registerAction<soul::KickState>(soul::AnimationState::Kick);
        soul::Kick2State* kick2State = inst->registerAction<soul::Kick2State>(soul::AnimationState::Kick2);
        soul::DefensiveState* defState = inst->registerAction<soul::DefensiveState>(soul::AnimationState::Defensive);
        soul::KnockedOutState* koState = inst->registerAction<soul::KnockedOutState>(soul::AnimationState::Knocked);

        idleState->defineDependencies(inst->stateManager);
        jumpState->defineDependencies(inst->stateManager);
        walkState->defineDependencies(inst->stateManager);
        actionIdleState->defineDependencies(inst->stateManager);
        punchState->defineDependencies(inst->stateManager);
        punchStickState->defineDependencies(inst->stateManager);
        shootState->defineDependencies(inst->stateManager);
        kickState->defineDependencies(inst->stateManager);
        kick2State->defineDependencies(inst->stateManager);
        defState->defineDependencies(inst->stateManager);
        koState->defineDependencies(inst->stateManager);

        inst->setAnimationState(soul::AnimationState::Idle);
        inst->setMovementState(*inst->getMovementState<soul::IdleState>(soul::AnimationState::Idle));
        inst->setActionState(*inst->getActionState<soul::ActionIdleState>(soul::AnimationState::ActionIdle));

        return inst;
    }
};

/**
 * @brief Loader class for Container
 * Class that loads data from a JSON file and creates a container instance of the specified types.
 */
template<typename T>
struct VectorLoader {
    static std::vector<std::shared_ptr<T>> load(const std::string& filePath, const int itemCount, Player& player) {
        static_assert(sizeof(T) == 0, "No vector loader defined for this type");
        return nullptr;
    }
};

/**
 * @brief VectorLoader specialization for Fireball
 * Loads Fireball data from a JSON file and creates a vector of instances.
 */
template<>
struct VectorLoader<Fireball> {
    static std::vector<std::shared_ptr<Fireball>> load(const std::string& jsonPath, const int itemCount, Player& player) {
        LoggerManager& logManager = LoggerManager::getInstance();
        // PathManager& pathManager = PathManager::getInstance();

        std::filesystem::path fullPath = jsonPath;
        std::string filename = fullPath.filename().string();
        if (!fullPath.has_extension()) {
            throw std::runtime_error(std::format("JSON file {} does not have a valid extension.", filename));
        }
        if (fullPath.extension() != ".json") {
            throw std::runtime_error(std::format("JSON file {} does not have a valid JSON extension.", filename));
        }

        logManager.log("Loading Fireball data from {}", jsonPath.data());

        std::ifstream file(jsonPath);
        json data;
        file >> data;

        if (!data.contains("fireballs") || !data["fireballs"].is_object()) {
            throw std::runtime_error("JSON for Player does not contain a valid 'fireballs' object.");
        }

        const auto& spriteJson = data["fireballs"];

        const soul::sSpriteData spriteData = {
            spriteJson["spritePath"].get<std::string>(), // Filename for the image file
            soul::Vector2f(spriteJson["scale"][0], spriteJson["scale"][1]), // Scaling the sprite from the image
            soul::Vector2i(spriteJson["spriteSheetPosition"][0], spriteJson["spriteSheetPosition"][1]), // Initial px position in the Sprite sheet
            soul::Vector2i(spriteJson["spriteSheetSize"][0], spriteJson["spriteSheetSize"][1]), // Initial px size in the Sprite sheet
            soul::Vector2f(0, 0), // Coordinates px of screen as they are defined 0,0 from top left.
            spriteJson["filterTransparency"], // Filter Transparency By Color
            soul::Color(
                spriteJson["filterColor"][0],
                spriteJson["filterColor"][1],
                spriteJson["filterColor"][2],
                spriteJson["filterColor"][3]) // Filtering Color
        };

        const soul::sTransformScalars scalars = {
            spriteJson["speedX"], // initialVelocityX
            spriteJson["speedY"], // initialVelocityY
            0.0f, // gravity
            1, // direction right by default
            spriteJson["angle"], // angle
            data["transformScalars"]["groundY"] // groundY
        };

        std::vector<std::shared_ptr<Fireball>> fireballs;

        for (auto id = 1; id <= itemCount; id++) {

            logManager.log("Create Fireball {}", id);

            auto inst = std::make_shared<Fireball>(
                player.getFireballSystem(),
                "Fireball",
                spriteData.initialPosition.x, 
                spriteData.initialPosition.y,
                data["fireballs"]["lifetime"],
                scalars.initialVelocityX,
                scalars.direction,
                scalars.angle);

            inst->load(spriteData, scalars);

            // Fireball is inactive until we shoot
            inst->setActive(false);

            auto animationFire = std::make_shared<SpriteAnimation>();

            auto duration = 0.05f;
            animationFire->addFrame(0, 0, 70, 70, duration);
            animationFire->addFrame(140, 0, 70, 70, duration);
            animationFire->addFrame(210, 0, 70, 70, duration);
            animationFire->addFrame(280, 0, 70, 70, duration);
            animationFire->addFrame(350, 0, 70, 70, duration);
            animationFire->addFrame(420, 0, 70, 70, duration);
            animationFire->addFrame(490, 0, 70, 70, duration);
            animationFire->addFrame(560, 0, 70, 70, duration);
            animationFire->addFrame(630, 0, 70, 70, duration);
            animationFire->addFrame(700, 0, 70, 70, duration);
            animationFire->addFrame(770, 0, 70, 70, duration);
            animationFire->addFrame(840, 0, 70, 70, duration);
            animationFire->addFrame(910, 0, 70, 70, duration);
            animationFire->addFrame(980, 0, 70, 70, duration);
            animationFire->addFrame(1050, 0, 70, 70, duration);
            
            inst->addAnimation(soul::AnimationState::ShootFireball, animationFire);
            
            // Add it in the Pool of instances ready to use
            fireballs.push_back(inst);
        }

        return fireballs;
    }
};

/**
 * @brief Loader specialization for Scene
 * Loads Scene data from a JSON file and creates an instance.
 */
template<>
struct Loader<Scene> {
    static std::unique_ptr<Scene> load(std::string_view jsonPath) {
        LoggerManager& logManager = LoggerManager::getInstance();

        std::filesystem::path fullPath = jsonPath;
        std::string filename = fullPath.filename().string();
        if (!fullPath.has_extension()) {
            throw std::runtime_error(std::format("JSON file {} does not have a valid extension.", filename));
        }
        if (fullPath.extension() != ".json") {
            throw std::runtime_error(std::format("JSON file {} does not have a valid JSON extension.", filename));
        }

        logManager.log("Loading Scene data from: ", jsonPath.data());
        
        // Load Scene data in the provided json file
        std::ifstream file(jsonPath.data());
        json data;
        file >> data;

        std::cout << "Scene Name: " << data["name"] << "\n";
        std::cout << "Description: " << data["description"] << "\n";

        auto inst = std::make_unique<Scene>(data["name"].get<std::string>());

        // std::vector<std::shared_ptr<Tile>> tiles;

        const auto& objects = data["objects"];

        for (const auto& obj : objects) {
            std::cout << "Object Name: " << obj["name"] << "\n";
            std::cout << "Object Type: " << obj["type"] << "\n";
            std::cout << "Texture Path: " << obj["filePath"] << "\n";

            std::cout << "  screenPosition: (" << obj["screenPosition"][0] << ", " << obj["screenPosition"][1] << ")\n";
            std::cout << "  spriteSheetPosition: (" << obj["spriteSheetPosition"][0] << ", " << obj["spriteSheetPosition"][1] << ")\n";
            std::cout << "  Scale: " << obj["scale"][0] << ", " << obj["scale"][1] << "\n";
            std::cout << "  Size: " << obj["spriteSheetSize"][0] << "x" << obj["spriteSheetSize"][1] << "\n";

            auto collision = obj["collision"];
            std::cout << "  Collision: " << std::boolalpha << collision << "\n";
    
            const soul::sSpriteData spriteData = {
                // Filename for the image file
                obj["filePath"],
                // Scaling the sprite from the image
                soul::Vector2f(obj["scale"][0], obj["scale"][1]),
                // Initial px position in the Sprite sheet
                soul::Vector2i(obj["spriteSheetPosition"][0], obj["spriteSheetPosition"][1]),
                // Initial px size in the Sprite sheet
                soul::Vector2i(obj["spriteSheetSize"][0], obj["spriteSheetSize"][1]),
                // Coordinates px of screen as they are defined 0,0 from top left.
                soul::Vector2f(obj["screenPosition"][0], obj["screenPosition"][1]), 
                // Filter Transparency By Color
                false, 
                // Filtering Color
                soul::Color(0, 0, 0, 0) // No color filter
            };

            std::shared_ptr<Entity> tile;
            tile = std::make_shared<Tile>(obj["name"], spriteData, collision);
            tile->setActive(true);
            logManager.logInfo("Tile {} {}", toString<uint32_t>(tile->ID()), tile->tag());
            
            inst->entities.addEntity(tile);
        }

        return inst;
    }
};

/**
 * @brief Loader specialization for Canvas
 * Loads Canvas data from a JSON file and creates an instance.
 */
template<>
struct SceneLoader<Canvas> {
    static std::unique_ptr<Canvas> load(std::string_view jsonPath, Scene& scene) {
        LoggerManager& logManager = LoggerManager::getInstance();
        PathManager& pathManager = PathManager::getInstance();

        std::filesystem::path fullPath = jsonPath;
        std::string filename = fullPath.filename().string();
        if (!fullPath.has_extension()) {
            throw std::runtime_error(std::format("JSON file {} does not have a valid extension.", filename));
        }
        if (fullPath.extension() != ".json") {
            throw std::runtime_error(std::format("JSON file {} does not have a valid JSON extension.", filename));
        }

        logManager.log("Loading Canvas data from: ", jsonPath.data());

        std::ifstream file(jsonPath.data());
        json data;
        file >> data;

        if (!data.contains("Global") || !data["Global"].is_object()) {
            throw std::runtime_error("JSON for Canvas does not contain a valid 'Global' object.");
        }

        if (!data.contains("Life") || !data["Life"].is_object()) {
            throw std::runtime_error("JSON for Canvas does not contain a valid 'Life' object.");
        }

        if (!data.contains("HealthBar") || !data["HealthBar"].is_object()) {
            throw std::runtime_error("JSON for Canvas does not contain a valid 'HealthBar' object.");
        }

        const std::string fontFilename = pathManager.getFilePath(PathManager::FileType::Font, data["Global"]["fontPath"])->string();

        CanvasProperties props;
        props.fontFilename = fontFilename;
        props.fontSize = data["Global"]["fontSize"];
        props.fontColor = sf::Color(data["Global"]["fontColor"][0], data["Global"]["fontColor"][1], data["Global"]["fontColor"][2]);

        props.hbarPosition = sf::Vector2f(data["HealthBar"]["position"][0], data["HealthBar"]["position"][1]);
        props.hbarWidth = data["HealthBar"]["width"];
        props.hbarHeight = data["HealthBar"]["height"];
        props.hbbgColor = sf::Color(data["HealthBar"]["backgroundColor"][0], data["HealthBar"]["backgroundColor"][1], data["HealthBar"]["backgroundColor"][2]);
        props.hbfgColor = sf::Color(data["HealthBar"]["foregroundColor"][0], data["HealthBar"]["foregroundColor"][1], data["HealthBar"]["foregroundColor"][2]);

        auto inst = std::make_unique<Canvas>("Canvas", scene);
        inst->load(props);

        return inst;
    }
};

} // namespace soul