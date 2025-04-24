#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Config.h"

class LeaderboardWindow {
private:
    sf::Font font;
    sf::Text titleText;
    sf::Text listText;

    std::vector<std::pair<std::string, std::string>> entries;
    std::string newName;
    std::string newTime;
    bool isNewEntry = false;

    int windowWidth;
    int windowHeight;

    void loadFile();
    void saveFile();

public:
    LeaderboardWindow(const Config& config, const std::string& playerName = "", const std::string& playerTime = "");
    void run();
};