#include "WelcomeWindow.h"
#include <cctype>
#include <algorithm>

WelcomeWindow::WelcomeWindow() {
    font.loadFromFile("files/font.ttf");

    titleText.setFont(font);
    titleText.setString("WELCOME TO MINESWEEPER!");
    titleText.setCharacterSize(24);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setFillColor(sf::Color::White);

    promptText.setFont(font);
    promptText.setString("Enter your name:");
    promptText.setCharacterSize(20);
    promptText.setFillColor(sf::Color::White);

    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Yellow);
}

void WelcomeWindow::updateInputDisplay() {
    if (!name.empty()) {
        std::string formatted = name;
        formatted[0] = std::toupper(formatted[0]);
        std::transform(formatted.begin() + 1, formatted.end(), formatted.begin() + 1, ::tolower);
        inputText.setString(formatted + "|");
    } else {
        inputText.setString("|");
    }
}

void WelcomeWindow::run() {
    sf::RenderWindow window(sf::VideoMode(800, 612), "Welcome", sf::Style::Close);
    window.setFramerateLimit(60);

    titleText.setPosition(400, 150);
    promptText.setPosition(400, 220);
    inputText.setPosition(400, 250);

    // center-align text
    for (sf::Text* t : { &titleText, &promptText, &inputText }) {
        sf::FloatRect rect = t->getLocalBounds();
        t->setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
    }

    updateInputDisplay();

    while (window.isOpen() && !finished) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!name.empty()) {
                        name.pop_back();
                        updateInputDisplay();
                    }
                } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                    if (!name.empty()) {
                        finished = true;
                        window.close();
                    }
                } else if (std::isalpha(static_cast<char>(event.text.unicode)) && name.length() < 10) {
                    name += static_cast<char>(event.text.unicode);
                    updateInputDisplay();
                }
            }
        }

        window.clear(sf::Color::Blue);
        window.draw(titleText);
        window.draw(promptText);
        window.draw(inputText);
        window.display();
    }
}

bool WelcomeWindow::isFinished() const {
    return finished;
}

std::string WelcomeWindow::getName() const {
    std::string formatted = name;
    if (!formatted.empty()) {
        formatted[0] = std::toupper(formatted[0]);
        std::transform(formatted.begin() + 1, formatted.end(), formatted.begin() + 1, ::tolower);
    }
    return formatted;
}