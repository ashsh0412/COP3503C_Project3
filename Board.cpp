#include "Board.h"
#include <cstdlib>
#include <ctime>

Board::Board(int rows, int cols, int mines)
    : rows(rows), cols(cols), mineCount(mines) {
    grid.resize(rows, std::vector<Tile>(cols));
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            grid[r][c] = Tile(c * 32.f, r * 32.f);
    placeMines();
    calculateAdjacents();
}

void Board::placeMines() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int placed = 0;
    while (placed < mineCount) {
        int r = std::rand() % rows;
        int c = std::rand() % cols;
        if (!grid[r][c].hasMine()) {
            grid[r][c].setMine(true);
            ++placed;
        }
    }
}

bool Board::inBounds(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

void Board::calculateAdjacents() {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c].hasMine()) continue;
            int count = 0;
            for (int dr = -1; dr <= 1; ++dr)
                for (int dc = -1; dc <= 1; ++dc) {
                    int nr = r + dr, nc = c + dc;
                    if (inBounds(nr, nc) && grid[nr][nc].hasMine())
                        ++count;
                }
            grid[r][c].setAdjacentMines(count);
        }
}

void Board::revealAdjacentZeros(int r, int c) {
    for (int dr = -1; dr <= 1; ++dr)
        for (int dc = -1; dc <= 1; ++dc) {
            int nr = r + dr, nc = c + dc;
            if (inBounds(nr, nc) && !grid[nr][nc].isTileRevealed() && !grid[nr][nc].hasMine()) {
                grid[nr][nc].reveal();
                if (grid[nr][nc].getAdjacentMines() == 0)
                    revealAdjacentZeros(nr, nc);
            }
        }
}

void Board::handleLeftClick(float x, float y) {
    if (gameOver || victory) return;
    int col = static_cast<int>(x) / 32;
    int row = static_cast<int>(y) / 32;
    if (!inBounds(row, col)) return;

    Tile& tile = grid[row][col];
    if (tile.isTileFlagged()) return;

    tile.reveal();
    if (tile.hasMine()) {
        gameOver = true;
        revealAllMines();
    } else if (tile.getAdjacentMines() == 0) {
        revealAdjacentZeros(row, col);
    }

    if (checkVictory()) {
        victory = true;
        flagAllMines();
    }
}

void Board::handleRightClick(float x, float y) {
    if (gameOver || victory) return;
    int col = static_cast<int>(x) / 32;
    int row = static_cast<int>(y) / 32;
    if (!inBounds(row, col)) return;

    grid[row][col].toggleFlag();
}

bool Board::checkVictory() const {
    for (const auto& row : grid)
        for (const auto& tile : row)
            if (!tile.hasMine() && !tile.isTileRevealed())
                return false;
    return true;
}

void Board::revealAllMines() {
    for (auto& row : grid)
        for (auto& tile : row)
            if (tile.hasMine())
                tile.reveal();
}

void Board::flagAllMines() {
    for (auto& row : grid)
        for (auto& tile : row)
            if (tile.hasMine())
                tile.setFlag(true);
}

void Board::draw(sf::RenderWindow& window) const {
    for (const auto& row : grid)
        for (const auto& tile : row)
            tile.draw(window);
}

Tile& Board::getTile(int row, int col) {
    return grid[row][col];
}

bool Board::isGameOver() const { return gameOver; }
bool Board::isVictory() const { return victory; }

void Board::reset() {
    gameOver = false;
    victory = false;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            grid[r][c] = Tile(c * 32.f, r * 32.f);
    placeMines();
    calculateAdjacents();
}

int Board::getFlagCount() const {
    int count = 0;
    for (const auto& row : grid) {
        for (const auto& tile : row) {
            if (tile.isTileFlagged())
                count++;
        }
    }
    return count;
}
void Board::draw(sf::RenderWindow& window, bool revealAll) const {
    for (const auto& row : grid) {
        for (const auto& tile : row) {
            tile.draw(window, revealAll);  // Tile도 revealAll 인자를 받는 버전으로 구현되어 있어야 함
        }
    }
}