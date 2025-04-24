#include "LeaderboardWindow.h"
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

LeaderboardWindow::LeaderboardWindow(const Config& config, const string& playerName, const string& playerTime) : newName(playerName), newTime(playerTime), windowWidth(config.getCols() * 16), windowHeight(config.getRows() * 16 + 50) {
    font.loadFromFile("files/font.ttf");

    // Title setting
    titleText.setFont(font);
    titleText.setString("LEADERBOARD");
    titleText.setCharacterSize(20);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setFillColor(sf::Color::White);

    // List setting
    listText.setFont(font);
    listText.setCharacterSize(18);
    listText.setFillColor(sf::Color::White);

    loadFile();

    // add new player
    if (!playerName.empty() && !playerTime.empty()) {
        entries.emplace_back(playerTime, playerName);
        sort(entries.begin(), entries.end());
        if (entries.size() > 5) entries.pop_back();
        isNewEntry = true;
        saveFile();
    }

    string leaderboardText = "";
    int currentRank = 1;

    for (const auto& [playerTime, playerName] : entries) {
        leaderboardText += to_string(currentRank) + ".    " + playerTime + "      " + playerName;

        if (isNewEntry && playerTime == newTime && playerName == newName) {
            leaderboardText += "*";
        }

        leaderboardText += "\n\n";
        currentRank++;
    }

    listText.setString(leaderboardText);

    // set texts middle
    float centerX = windowWidth / 2.0f;
    float centerY = windowHeight / 2.0f;

    // 5.4 additional notes, 7.4 formatting and output
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
    titleText.setPosition(centerX, centerY - 120);

    sf::FloatRect listBounds = listText.getLocalBounds();
    listText.setOrigin(listBounds.left + listBounds.width / 2.0f, listBounds.top + listBounds.height / 2.0f);
    listText.setPosition(centerX, centerY + 20);
}

void LeaderboardWindow::loadFile() {
    ifstream file("files/leaderboard.txt");

    string line;
    while (getline(file, line)) {
        stringstream ss(line);

        string time;
        string name;

        getline(ss, time, ',');
        getline(ss, name);

        if (!time.empty() && !name.empty()) {
            entries.emplace_back(time, name);
        }
    }
}

void LeaderboardWindow::saveFile() {
    ofstream file("files/leaderboard.txt");

    int count = min(5, static_cast<int>(entries.size()));

    for (int i = 0; i < count; i++) {
        string time = entries[i].first;
        string name = entries[i].second;

        file << time << "," << name << "\n";
    }
}

void LeaderboardWindow::run() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Minesweeper", sf::Style::Close);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Blue);
        window.draw(titleText);
        window.draw(listText);
        window.display();
    }
}