#include "TextureManager.h"

bool TextureManager::load(const std::string& name, const std::string& filepath) {
    sf::Texture texture;
    if (!texture.loadFromFile(filepath)) {
        return false;
    }
    textures[name] = std::move(texture);
    return true;
}

sf::Texture* TextureManager::get(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return &it->second;
    }
    return nullptr;
}