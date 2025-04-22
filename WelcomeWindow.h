#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class WelcomeWindow {
private:
    sf::Font font;
    sf::Text titleText;
    sf::Text promptText;
    sf::Text inputText;
    std::string name;
    bool finished = false;

    void updateInputDisplay();

public:
    WelcomeWindow();
    void run();
    bool isFinished() const;
    std::string getName() const;
};