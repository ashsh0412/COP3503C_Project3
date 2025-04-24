#include "WelcomeWindow.h"
#include "Config.h"
#include "GameWindow.h"

int main() {
    Config config("files/config.cfg");
    WelcomeWindow welcome(config);
    welcome.run();

    if (welcome.isFinished()) {
        GameWindow game(config, welcome.getName());
        game.run();
    }
}