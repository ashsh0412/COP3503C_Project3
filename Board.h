#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include <vector>

class Board {
private:
    std::vector<std::vector<Tile>> grid;
    int rows, cols, mineCount;
    bool gameOver = false;
    bool victory = false;

    void placeMines();
    void calculateAdjacents();
    void revealAdjacentZeros(int row, int col);
    void revealAllMines();
    void flagAllMines();
    bool inBounds(int row, int col) const;
    bool checkVictory() const; // ✅ 이거까지 포함!

public:
    Board(int rows, int cols, int mines);
    void handleLeftClick(float x, float y);
    void handleRightClick(float x, float y);
    void draw(sf::RenderWindow& window) const;
    Tile& getTile(int row, int col);

    bool isGameOver() const;
    bool isVictory() const;
    int getFlagCount() const;
    void draw(sf::RenderWindow& window, bool revealAll = false) const; // ✅ 이 줄 추가
    void reset();
};