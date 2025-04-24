#include "Board.h"
#include <cstdlib>
#include <ctime>
using namespace std;
Board::Board(int rowCount, int colCount, int totalMines) : rows(rowCount), cols(colCount), mineCount(totalMines) {

    for (int row = 0; row < rows; row++) {
        vector<Tile> oneRow;

        for (int col = 0; col < cols; col++) {
            float tileX = col * 32.0f;
            float tileY = row * 32.0f;

            Tile tile(tileX, tileY);
            oneRow.push_back(tile);
        }

        grid.push_back(oneRow);
    }

    placeMines();
    calculateAdjacents();
}
// refer to https://cplusplus.com/reference/cstdlib/rand/
void Board::placeMines() {
    srand(static_cast<unsigned>(time(nullptr)));
    int placed = 0;
    while (placed < mineCount) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (!grid[r][c].hasMine()) {
            grid[r][c].setMine(true);
            ++placed;
        }
    }
}
// check cursor is in tiles
bool Board::inBounds(int row, int col) const {
    bool rowValid = (row >= 0) && (row < rows);
    bool colValid = (col >= 0) && (col < cols);

    return rowValid && colValid;
}
// refer to https://stackoverflow.com/questions/33110605/im-building-a-minesweeper-project-and-i-need-help-counting-adjacent-mines
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
// refer to https://apcs.tinocs.com/lesson/FX7_Minesweeper/B.md, https://www.youtube.com/watch?v=it5eRaMHJTo&ab_channel=TedMcLeod
void Board::revealAdjacentZeros(int r, int c) {
    for (int dr = -1; dr <= 1; ++dr)
        for (int dc = -1; dc <= 1; ++dc) {
            int nr = r + dr, nc = c + dc;
            if (inBounds(nr, nc)) {
                Tile& tile = grid[nr][nc];
                if (!tile.isTileRevealed() && !tile.isTileFlagged() && !tile.hasMine()) {
                    tile.reveal();
                    if (tile.getAdjacentMines() == 0)
                        revealAdjacentZeros(nr, nc);
                }
            }
        }
}
// reveal tiles
void Board::handleLeftClick(float x, float y) {
    int col = static_cast<int>(x) / 32;
    int row = static_cast<int>(y) / 32;

    // not in game page
    if (!inBounds(row, col)) return;

    Tile& tile = grid[row][col];
    // not working if flagged
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
// only flag or unflag
void Board::handleRightClick(float x, float y) {
    int col = static_cast<int>(x) / 32;
    int row = static_cast<int>(y) / 32;
    if (!inBounds(row, col)) return;

    Tile& tile = grid[row][col];
    if (!tile.isTileRevealed()) {
        tile.toggleFlag();
    }
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
// flag all mine tiles and unflag all un-mined tiles
void Board::flagAllMines() {
    for (auto& row : grid)
        for (auto& tile : row) {
            if (tile.hasMine())
                tile.setFlag(true);
            else if (tile.isTileFlagged())
                tile.setFlag(false);
        }
}

Tile& Board::getTile(int row, int col) {
    return grid[row][col];
}

bool Board::isGameOver() const { return gameOver; }
bool Board::isVictory() const { return victory; }

void Board::reset() {
    gameOver = false;
    victory = false;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {

            float tileX = col * 32.0f;
            float tileY = row * 32.0f;

            Tile newTile(tileX, tileY);

            grid[row][col] = newTile;
        }
    }
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

void Board::draw(sf::RenderWindow& window, bool revealPaused, bool revealMines) const {
    for (const auto& row : grid)
        for (const auto& tile : row)
            tile.draw(window, revealPaused, revealMines);
}