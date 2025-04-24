#include "WelcomeWindow.h"
#include <cctype>
using namespace std;

WelcomeWindow::WelcomeWindow(const Config& config) : windowWidth(config.getCols() * 32), windowHeight(config.getRows() * 32 + 100) {
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
    string displayName = getName();

    inputText.setString(displayName + "|");

    // 5.4 additional notes
    sf::FloatRect bounds = inputText.getLocalBounds();
    inputText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}

void WelcomeWindow::run() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Welcome", sf::Style::Close);

    titleText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f - 150);
    promptText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f - 75);
    inputText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f - 45);

    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);

    sf::FloatRect promptBounds = promptText.getLocalBounds();
    promptText.setOrigin(promptBounds.left + promptBounds.width / 2.0f, promptBounds.top + promptBounds.height / 2.0f);

    sf::FloatRect inputBounds = inputText.getLocalBounds();
    inputText.setOrigin(inputBounds.left + inputBounds.width / 2.0f, inputBounds.top + inputBounds.height / 2.0f);

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

                } else if (isalpha(static_cast<char>(event.text.unicode)) && name.length() < 10) {
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

string WelcomeWindow::getName() const {
    string formatted = name;
    if (!formatted.empty()) {
        formatted[0] = toupper(formatted[0]);
        transform(formatted.begin() + 1, formatted.end(), formatted.begin() + 1, ::tolower);
    }
    return formatted;
}