#include "WelcomeWindow.h"
#include "Config.h"
#include "GameWindow.h"

int main() {
    WelcomeWindow welcome;
    welcome.run();

    if (welcome.isFinished()) {
        Config config("files/config.cfg");
        GameWindow game(config, welcome.getName());  // ✅ 이름도 전달
        game.run();
    }

    return 0;
}