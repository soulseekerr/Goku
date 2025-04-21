#pragma once

#include <memory>
#include <functional>
#include <cstddef>
#include <cstdint>
#include <map>

#include "singleton.h"
#include "hashset.h"
#include "texture2d.h"

namespace soul {

class Color;

struct StringHash {
    std::size_t operator()(const std::string& key) const {
        return std::hash<std::string>()(key); // Hash based on ID
    }
};

class AssetManager : public SingletonT<AssetManager> {
    MAKE_SINGLETON(AssetManager)

public:
    std::shared_ptr<Texture2d>& getTexture(const std::string& name);

    std::shared_ptr<Texture2d>& addTexture(const std::string& name, const std::string& file_path, bool is_smooth);

    std::shared_ptr<Texture2d>& addTextureImageFilter(const std::string& name, const std::string& file_path, bool is_smooth, const soul::Color& backgroundColor);

    size_t getCountTextures() const;

private:
    HashSet_t<std::string, StringHash> textureKeys;
    std::map<std::string, std::shared_ptr<Texture2d>> mapTextures;
};

} // namespace soul