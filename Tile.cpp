#include "Tile.h"

Tile::Tile(float x, float y) {
    baseSprite.setPosition(x, y);
}

void Tile::setTexture(sf::Texture& texture) {
    baseSprite.setTexture(texture);
}

void Tile::setPosition(float x, float y) {
    baseSprite.setPosition(x, y);
    flagSprite.setPosition(x, y);
    mineSprite.setPosition(x, y);  // ✅ 폭탄 위치도 동기화
    for (int i = 1; i <= 8; ++i)
        numberSprites[i].setPosition(x, y);
}

void Tile::setFlagTexture(sf::Texture& texture) {
    flagTexture = &texture;
    flagSprite.setTexture(texture);
}

void Tile::setMineTexture(sf::Texture& texture) {
    mineTexture = &texture;
    mineSprite.setTexture(texture);
    mineSprite.setPosition(baseSprite.getPosition());  // ✅ 현재 타일 위치로 동기화
}

void Tile::setNumberTexture(int num, sf::Texture& texture) {
    if (num >= 1 && num <= 8) {
        numberTextures[num] = &texture;
        numberSprites[num].setTexture(texture);

        sf::Vector2f pos = baseSprite.getPosition();
        numberSprites[num].setPosition(pos);
    }
}

void Tile::setMine(bool mine) {
    isMine = mine;
}

void Tile::setFlag(bool value) {
    if (!isRevealed) isFlagged = value;
}

void Tile::toggleFlag() {
    if (!isRevealed) isFlagged = !isFlagged;
}

void Tile::reveal() {
    if (!isFlagged) isRevealed = true;
}

void Tile::setAdjacentMines(int count) {
    adjacentMines = count;
}

int Tile::getAdjacentMines() const { return adjacentMines; }
bool Tile::hasMine() const { return isMine; }
bool Tile::isTileRevealed() const { return isRevealed; }
bool Tile::isTileFlagged() const { return isFlagged; }

void Tile::draw(sf::RenderWindow& window, bool revealAll) const {
    if (revealAll) {
        sf::Sprite revealed = baseSprite;
        if (adjacentMines > 0 && numberTextures[adjacentMines])
            revealed.setTexture(*numberTextures[adjacentMines]);
        else
            revealed.setTexture(baseSprite.getTexture() ? *baseSprite.getTexture() : sf::Texture());
        window.draw(revealed);
    } else {
        window.draw(baseSprite);
        if (isFlagged && !isRevealed && flagTexture)
            window.draw(flagSprite);
        else if (isRevealed && isMine && mineTexture)
            window.draw(mineSprite);
        else if (isRevealed && adjacentMines > 0 && numberTextures[adjacentMines])
            window.draw(numberSprites[adjacentMines]);
    }
}