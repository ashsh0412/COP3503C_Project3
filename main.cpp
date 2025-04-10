#include <SFML/Graphics.hpp>
#include <iostream>
#include <cctype>

void centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    text.setPosition(x, y);
}

int main() {
    int columns = 25;
    int rows = 16;
    int tileSize = 32;
    int width = columns * tileSize;
    int height = rows * tileSize + 100;

    sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Welcome", sf::Style::Close);
    welcomeWindow.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        return -1;
    }

    sf::Text title("WELCOME TO MINESWEEPER!", font, 24);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::White);
    centerText(title, width / 2.0f, height / 2.0f - 150);

    sf::Text prompt("Enter your name:", font, 20);
    prompt.setStyle(sf::Text::Bold);
    prompt.setFillColor(sf::Color::White);
    centerText(prompt, width / 2.0f, height / 2.0f - 75);

    sf::Text nameText;
    nameText.setFont(font);
    nameText.setCharacterSize(18);
    nameText.setFillColor(sf::Color::Yellow);

    std::string name;
    bool cursorVisible = true;
    sf::Clock cursorTimer;

    while (welcomeWindow.isOpen()) {
        sf::Event event;
        while (welcomeWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                welcomeWindow.close();
                return 0;
            }

            if (event.type == sf::Event::TextEntered) {
                char ch = static_cast<char>(event.text.unicode);
                if (ch == 8 && !name.empty()) {
                    name.pop_back();
                } else if (std::isalpha(ch) && name.length() < 10) {
                    name += ch;
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (!name.empty()) {
                    welcomeWindow.close();
                    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper - " + name);
                    while (gameWindow.isOpen()) {
                        sf::Event gameEvent;
                        while (gameWindow.pollEvent(gameEvent)) {
                            if (gameEvent.type == sf::Event::Closed) {
                                gameWindow.close();
                            }
                        }
                        gameWindow.clear(sf::Color::White);
                        gameWindow.display();
                    }
                }
            }
        }

        std::string displayName = name;
        if (!displayName.empty()) {
            displayName[0] = std::toupper(displayName[0]);
            for (size_t i = 1; i < displayName.size(); ++i) {
                displayName[i] = std::tolower(displayName[i]);
            }
        }

        nameText.setString(displayName + (cursorVisible ? "|" : ""));
        centerText(nameText, width / 2.0f, height / 2.0f - 45);

        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(title);
        welcomeWindow.draw(prompt);
        welcomeWindow.draw(nameText);
        welcomeWindow.display();
    }

    return 0;
}
