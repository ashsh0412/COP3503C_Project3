#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class TextureManager {
private:
    std::unordered_map<std::string, sf::Texture> textures;

public:
    bool load(const std::string& name, const std::string& filepath);
    sf::Texture* get(const std::string& name);
};