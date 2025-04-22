#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class LeaderboardWindow {
private:
    sf::Font font;
    sf::Text titleText;
    sf::Text listText;

    std::vector<std::pair<std::string, std::string>> entries; // time, name
    std::string newName;
    std::string newTime;
    bool isNewEntry = false;

    void loadFile();
    void saveFile();

public:
    LeaderboardWindow(const std::string& playerName = "", const std::string& playerTime = "");
    void run();
};