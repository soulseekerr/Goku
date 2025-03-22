#pragma once

#include "singleton.h"
#include "logger.h"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <optional>

namespace fs = std::filesystem;

namespace soul {

class PathManager : public SingletonT<PathManager> {
    MAKE_SINGLETON(PathManager)

public:
    enum class FileType { Texture, Font, Config, Test };

private:
    // ordering not necessary, unordered_map is faster than map, O(1) average vs O(log n)
    std::unordered_map<FileType, fs::path> directories;
    fs::path projectRoot;

public:
    void onInit() override {
        auto& log = LoggerManager::getInstance();

        const char* envRoot = std::getenv("SOUL_PROJECT_ROOT");
        if (envRoot) {
            projectRoot = std::string(envRoot);
        } else {
            // Use the current working directory as a fallback
            projectRoot = std::filesystem::current_path() / "data";
        }

        // Validate project root directory
        if (!std::filesystem::exists(projectRoot / "config")) {
            log.logWarn("Warning: Config folder not found in {}", projectRoot.c_str());
        }

        loadConfig(); // Load paths from config.json
    }

    // Returns the full path to the config.json file
    std::string getConfigPath() const {
        auto& log = LoggerManager::getInstance();
        std::filesystem::path configPath = projectRoot / "config.json";

        if (!std::filesystem::exists(configPath)) {
            log.logWarn("Warning: Config file not found at {}", configPath.c_str());
        }

        return configPath.string();
    }

    /// Get the full path of a file based on category
    std::optional<fs::path> getFilePath(FileType type, const std::string& filename) const {
        auto it = directories.find(type);
        if (it != directories.end()) {
            return projectRoot / it->second / filename;
        }
        return std::nullopt; // Return empty if type not found
    }

    /// Get any file in the project folder (generic usage)
    fs::path getFullPath(const std::string& relativePath) const {
        return projectRoot / relativePath;
    }

    // Load paths from a JSON config file
    void loadConfig() {
        auto& log = LoggerManager::getInstance();
        fs::path configFile = getConfigPath();
        if (!fs::exists(configFile)) {
            log.logWarn("Config file missing, generating default...");
            createDefaultConfig(configFile);
        }

        std::ifstream file(configFile);
        if (!file) {
            log.logWarn("Failed to open config file: {}", configFile.c_str());
            return;
        }

        nlohmann::json config;
        file >> config;

        directories[FileType::Texture] = config.value("textures", "textures");
        directories[FileType::Font] = config.value("fonts", "fonts");
        directories[FileType::Config] = config.value("config", "config");
    }

private:
    /// Creates a default config.json if it doesn't exist
    void createDefaultConfig(const fs::path& path) {
        auto& log = LoggerManager::getInstance();

        nlohmann::json defaultConfig = {
            {"textures", "textures"},
            {"fonts", "fonts"},
            {"config", "config"}
        };

        std::ofstream file(path);
        if (file) {
            file << defaultConfig.dump(4); // Pretty-print JSON
            log.logInfo("Default config.json created at {}", path.c_str());
        } else {
            log.logInfo("Failed to create config file at ", path.c_str());
        }
    }
};

} // namespace soul