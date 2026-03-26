#pragma once
#include "core/ResourceManager.hpp"
#include "glm/ext/vector_float2.hpp"
#include <memory>
struct Sprite {
    glm::vec2 position;
    glm::vec2 size;
    std::shared_ptr<Texture> texture;
    bool visable;
};
