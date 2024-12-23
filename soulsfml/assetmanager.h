#pragma once

#include <memory>
#include <functional>
#include <cstddef>
#include <cstdint>
#include <map>

#include "core/singleton.h"
#include "core/hashset.h"
#include "texture2d.h"

namespace soul {

struct StringHash {
    std::size_t operator()(const std::string& key) const {
        return std::hash<std::string>()(key); // Hash based on ID
    }
};

class AssetManager : public SingletonT<AssetManager> {
    MAKE_SINGLETON(AssetManager)

public:
    shared_ptr<Texture2d>& getTexture(const std::string& name);

    shared_ptr<Texture2d>& addTexture(const std::string& name, const std::string& file_path, bool is_smooth);

    shared_ptr<Texture2d>& addTextureImageFilter(const std::string& name, const std::string& file_path, bool is_smooth, sf::Color backgroundColor);

    const size_t getCountTextures() const;

private:
    HashSet_t<std::string, StringHash> textureKeys;
    map<std::string, std::shared_ptr<Texture2d>> mapTextures;
};

} // namespace soul