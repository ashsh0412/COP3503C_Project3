#include "LeaderboardWindow.h"
#include <fstream>
#include <sstream>
#include <algorithm>

LeaderboardWindow::LeaderboardWindow(const std::string& playerName, const std::string& playerTime)
    : newName(playerName), newTime(playerTime) {
    font.loadFromFile("files/font.ttf");

    titleText.setFont(font);
    titleText.setString("LEADERBOARD");
    titleText.setCharacterSize(20);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setFillColor(sf::Color::White);

    listText.setFont(font);
    listText.setCharacterSize(18);
    listText.setFillColor(sf::Color::White);

    loadFile();

    if (!playerName.empty() && !playerTime.empty()) {
        entries.emplace_back(playerTime, playerName);
        std::sort(entries.begin(), entries.end()); // time 오름차순 정렬
        if (entries.size() > 5) entries.pop_back();

        // * 표시용용은 entries에만 반영
        isNewEntry = true;
        saveFile(); // 저장할 때는 * 없이 저장
    }

    // 텍스트 형식 구성
    std::ostringstream oss;
    int rank = 1;
    for (const auto& [time, name] : entries) {
        oss << rank << ".\t" << time << ", " << name;
        if (isNewEntry && time == newTime && name == newName)
            oss << "*";
        oss << "\n\n";
        ++rank;
    }

    listText.setString(oss.str());

    // 중앙 정렬
    sf::FloatRect rect = titleText.getLocalBounds();
    titleText.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
    titleText.setPosition(400, 100);

    rect = listText.getLocalBounds();
    listText.setOrigin(rect.left + rect.width / 2.0f, rect.top);
    listText.setPosition(400, 160);
}

void LeaderboardWindow::loadFile() {
    std::ifstream file("files/leaderboard.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string time, name;
        if (std::getline(ss, time, ',') && std::getline(ss, name)) {
            entries.emplace_back(time, name);
        }
    }
}

void LeaderboardWindow::saveFile() {
    std::ofstream file("files/leaderboard.txt");
    for (int i = 0; i < std::min(5, static_cast<int>(entries.size())); ++i) {
        file << entries[i].first << "," << entries[i].second << "\n";
    }
}

void LeaderboardWindow::run() {
    sf::RenderWindow window(sf::VideoMode(800, 612), "Leaderboard", sf::Style::Close);
    window.setFramerateLimit(60);

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