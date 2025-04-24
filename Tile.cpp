#include "Tile.h"

// &texture --> getting from other codes.

Tile::Tile(float x, float y) {
    baseSprite.setPosition(x, y);
    flagSprite.setPosition(x, y);
    mineSprite.setPosition(x, y);

    // set numbers
    for (int i = 1; i <= 8; ++i)
        numberSprites[i].setPosition(x, y);
}

void Tile::setTexture(sf::Texture& texture) {
    baseSprite.setTexture(texture);
}

void Tile::setPosition(float x, float y) {
    baseSprite.setPosition(x, y);
    flagSprite.setPosition(x, y);
    mineSprite.setPosition(x, y);

    // set numbers
    for (int i = 1; i <= 8; ++i)
        numberSprites[i].setPosition(x, y);
}

void Tile::setFlagTexture(sf::Texture& texture) {
    flagTexture = &texture;
    flagSprite.setTexture(texture);
    flagSprite.setPosition(baseSprite.getPosition());
}

void Tile::setMineTexture(sf::Texture& texture) {
    mineTexture = &texture;
    mineSprite.setTexture(texture);
    mineSprite.setPosition(baseSprite.getPosition());
}

void Tile::setRevealedTexture(sf::Texture& texture) {
    revealedTexture = &texture;
}

void Tile::setNumberTexture(int num, sf::Texture& texture) {
    if (num >= 1 && num <= 8) {
        numberTextures[num] = &texture;
        numberSprites[num].setTexture(texture);
        numberSprites[num].setPosition(baseSprite.getPosition());
    }
}

void Tile::setMine(bool mine) {
    isMine = mine;
}

void Tile::setFlag(bool value) {
    if (!isRevealed) {
        isFlagged = value;
    }
}

void Tile::toggleFlag() {
    if (!isRevealed) {
        isFlagged = !isFlagged;
    }
}

void Tile::reveal() {
    if (!isFlagged) {
        isRevealed = true;
    }
}

void Tile::setAdjacentMines(int count) {
    adjacentMines = count;
}

int Tile::getAdjacentMines() const { return adjacentMines; }
bool Tile::hasMine() const { return isMine; }
bool Tile::isTileRevealed() const { return isRevealed; }
bool Tile::isTileFlagged() const { return isFlagged; }

void Tile::draw(sf::RenderWindow& window, bool revealPaused, bool revealMines) const {
    window.draw(baseSprite);

    // paused play
    if (revealPaused) {
        if (revealedTexture) {
            sf::Sprite revealed;
            revealed.setTexture(*revealedTexture);
            revealed.setPosition(baseSprite.getPosition());
            window.draw(revealed);
        }
        return;
    }

    // flag
    if (isFlagged && !isRevealed && flagTexture) {
        window.draw(flagSprite);
    }

    if (isRevealed) {
        if (revealedTexture) {
            sf::Sprite revealed;
            revealed.setTexture(*revealedTexture);
            revealed.setPosition(baseSprite.getPosition());
            window.draw(revealed);
        }

        if (isMine && mineTexture) {
            window.draw(mineSprite);
        } else if (adjacentMines > 0 && numberTextures[adjacentMines]) {
            window.draw(numberSprites[adjacentMines]);
        }
    }

    // debug mode
    else if (!isRevealed && isMine && revealMines && mineTexture) {
        window.draw(mineSprite);
    }
}