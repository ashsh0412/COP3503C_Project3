#pragma once
#include "Config.h"
#include "Board.h"
#include "TextureManager.h"
#include "TimerPause.h"
#include <SFML/Graphics.hpp>
#include <string>

class GameWindow {
private:
    Config& config;
    Board board;
    TextureManager textures;
    TimerPause timer;

    std::string playerName;
    int flagCount = 0;
    bool isPaused = false;
    bool debugMode = false;
    bool victoryHandled = false;

    sf::Sprite faceButton;
    sf::Sprite leaderboardButton;
    sf::Sprite pauseButton;
    sf::Sprite debugButton;

    void loadTextures();
    void setupTiles();
    void updateFace();
    void drawTimer(sf::RenderWindow& window);
    void drawFlagCounter(sf::RenderWindow& window);
    void togglePause();
    void openLeaderboard(const std::string& time = "", bool win = false);
    std::string formatTime(int sec) const;

public:
    GameWindow(Config& config, const std::string& name);
    void run();
};