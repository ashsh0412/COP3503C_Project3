#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Config.h"

class WelcomeWindow {
private:
    sf::Font font;
    sf::Text titleText;
    sf::Text promptText;
    sf::Text inputText;
    std::string name;
    bool finished = false;

    int windowWidth;
    int windowHeight;

    void updateInputDisplay();

public:
    WelcomeWindow(const Config& config);
    void run();
    bool isFinished() const;
    std::string getName() const;
};