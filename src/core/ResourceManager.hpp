#pragma once
#include "renderer/Texture.hpp"
#include <memory>
#include <string>
#include <unordered_map>
class ResourceManager {
  private:
    std::unordered_map<std::string, std::shared_ptr<Texture> > TextureCache;
    ResourceManager();

  public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    static ResourceManager& Instance() {
        static ResourceManager instance;
        return instance;
    }
    std::shared_ptr<Texture> Get(const std::string& path);
};
