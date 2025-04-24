#include "TextureManager.h"
using namespace std;
// shoutout to https://github.com/azimjohn/minesweeper/blob/master/texture_manager.cpp
bool TextureManager::load(const string& name, const string& filePath) {
    sf::Texture newTexture;

    if (!newTexture.loadFromFile(filePath)) {
        return false;
    }
    textures[name] = newTexture;
    return true;
}

sf::Texture* TextureManager::get(const string& name) {
    if (textures.count(name) > 0) {
        return &textures[name];
    }
    return nullptr;
}