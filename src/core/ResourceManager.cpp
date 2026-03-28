#include "ResourceManager.hpp"
#include <memory>

ResourceManager::ResourceManager() : TextureCache() {}

std::shared_ptr<Texture> ResourceManager::Get(const std::string& path) {
    auto it = TextureCache.find(path);
    if (it != TextureCache.end()) {
        return it->second;
    }
    auto t = std::make_shared<Texture>(path);
    TextureCache[path] = t;
    return t;
}

void ResourceManager::Shutdown() { TextureCache.clear(); }
