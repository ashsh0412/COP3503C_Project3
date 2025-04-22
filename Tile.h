#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class Tile {
private:
    sf::Sprite baseSprite;
    sf::Sprite flagSprite;
    sf::Sprite mineSprite;
    std::array<sf::Sprite, 9> numberSprites;

    const sf::Texture* flagTexture = nullptr;
    const sf::Texture* mineTexture = nullptr;
    std::array<const sf::Texture*, 9> numberTextures{};

    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int adjacentMines = 0;

public:
    Tile(float x = 0, float y = 0);

    void setTexture(sf::Texture& texture);
    void setFlagTexture(sf::Texture& texture);
    void setMineTexture(sf::Texture& texture);
    void setNumberTexture(int num, sf::Texture& texture);

    void setPosition(float x, float y);
    void setMine(bool mine);
    void setFlag(bool value);
    void toggleFlag();
    void reveal();
    void setAdjacentMines(int count);

    int getAdjacentMines() const;
    bool hasMine() const;
    bool isTileRevealed() const;
    bool isTileFlagged() const;
    void draw(sf::RenderWindow& window, bool revealAll = false) const;
};