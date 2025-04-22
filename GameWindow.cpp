#include "GameWindow.h"
#include "LeaderboardWindow.h"

GameWindow::GameWindow(Config& config, const std::string& name)
    : config(config), board(config.getRows(), config.getCols(), config.getMines()), playerName(name) {}

void GameWindow::loadTextures() {
    textures.load("tile_hidden", "files/images/tile_hidden.png");
    textures.load("tile_revealed", "files/images/tile_revealed.png");
    textures.load("flag", "files/images/flag.png");
    textures.load("mine", "files/images/mine.png");
    for (int i = 1; i <= 8; ++i)
        textures.load("number_" + std::to_string(i), "files/images/number_" + std::to_string(i) + ".png");
    textures.load("face_happy", "files/images/face_happy.png");
    textures.load("face_win", "files/images/face_win.png");
    textures.load("face_lose", "files/images/face_lose.png");
    textures.load("pause", "files/images/pause.png");
    textures.load("play", "files/images/play.png");
    textures.load("debug", "files/images/debug.png");
    textures.load("leaderboard", "files/images/leaderboard.png");
    textures.load("digits", "files/images/digits.png");
}

void GameWindow::setupTiles() {
    flagCount = 0;
    for (int r = 0; r < config.getRows(); ++r) {
        for (int c = 0; c < config.getCols(); ++c) {
            Tile& tile = board.getTile(r, c);
            tile.setTexture(*textures.get("tile_hidden"));
            tile.setFlagTexture(*textures.get("flag"));
            tile.setMineTexture(*textures.get("mine"));
            for (int i = 1; i <= 8; ++i)
                tile.setNumberTexture(i, *textures.get("number_" + std::to_string(i)));
        }
    }

    faceButton.setTexture(*textures.get("face_happy"));
    faceButton.setPosition(config.getCols() * 32 / 2 - 32, config.getRows() * 32 + 32);

    leaderboardButton.setTexture(*textures.get("leaderboard"));
    leaderboardButton.setPosition(config.getCols() * 32 - 176, config.getRows() * 32 + 32);

    pauseButton.setTexture(*textures.get("pause"));
    pauseButton.setPosition(config.getCols() * 32 - 240, config.getRows() * 32 + 32);

    debugButton.setTexture(*textures.get("debug"));
    debugButton.setPosition(config.getCols() * 32 - 304, config.getRows() * 32 + 32);
}
void GameWindow::updateFace() {
    if (board.isVictory())
        faceButton.setTexture(*textures.get("face_win"));
    else if (board.isGameOver())
        faceButton.setTexture(*textures.get("face_lose"));
    else
        faceButton.setTexture(*textures.get("face_happy"));
}

void GameWindow::drawTimer(sf::RenderWindow& window) {
    int seconds = timer.getElapsedSeconds();
    int min = seconds / 60;
    int sec = seconds % 60;

    sf::Texture* digits = textures.get("digits");
    if (!digits) return;

    int x = config.getCols() * 32 - 97;
    int y = config.getRows() * 32 + 48;

    auto drawDigit = [&](int num, int dx) {
        sf::Sprite sprite;
        sprite.setTexture(*digits);
        sprite.setTextureRect(sf::IntRect(num * 21, 0, 21, 32));
        sprite.setPosition((float)(x + dx), (float)y);
        window.draw(sprite);
    };

    drawDigit(min / 10, 0);
    drawDigit(min % 10, 21);
    drawDigit(sec / 10, 21 * 2 + 12);
    drawDigit(sec % 10, 21 * 3 + 12);
}

void GameWindow::drawFlagCounter(sf::RenderWindow& window) {
    int mines = config.getMines() - board.getFlagCount(); // flagCount 동기화 필요
    sf::Texture* digits = textures.get("digits");
    if (!digits) return;

    int x = 33;
    int y = config.getRows() * 32 + 48;

    if (mines < 0) {
        sf::Sprite dash;
        dash.setTexture(*digits);
        dash.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32)); // '-' 기호는 digits에서 10번째 위치
        dash.setPosition((float)x, (float)y);
        window.draw(dash);
        mines = -mines;
        x += 21;
    }

    auto drawDigit = [&](int num) {
        sf::Sprite sprite;
        sprite.setTexture(*digits);
        sprite.setTextureRect(sf::IntRect(num * 21, 0, 21, 32));
        sprite.setPosition((float)x, (float)y);
        window.draw(sprite);
        x += 21;
    };

    drawDigit(mines / 10);
    drawDigit(mines % 10);
}

void GameWindow::togglePause() {
    isPaused = !isPaused;
    if (isPaused) {
        pauseButton.setTexture(*textures.get("play"));
        timer.pause();
    } else {
        pauseButton.setTexture(*textures.get("pause"));
        timer.resume();
    }
}

std::string GameWindow::formatTime(int sec) const {
    int m = sec / 60, s = sec % 60;
    char buf[6];
    snprintf(buf, sizeof(buf), "%02d:%02d", m, s);
    return std::string(buf);
}

void GameWindow::openLeaderboard(const std::string& time, bool win) {
    if (win)
        LeaderboardWindow(playerName, time).run();
    else
        LeaderboardWindow().run();
}
void GameWindow::run() {
    int width = config.getCols() * 32;
    int height = config.getRows() * 32 + 100;

    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    window.setFramerateLimit(60);

    loadTextures();
    setupTiles();
    timer.start();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                float x = (float)sf::Mouse::getPosition(window).x;
                float y = (float)sf::Mouse::getPosition(window).y;

                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (faceButton.getGlobalBounds().contains(x, y)) {
                        board.reset();
                        setupTiles();
                        timer.reset();
                        timer.start();
                        victoryHandled = false;
                        isPaused = false;
                        pauseButton.setTexture(*textures.get("pause"));
                    } else if (leaderboardButton.getGlobalBounds().contains(x, y)) {
                        timer.pause();
                        openLeaderboard();
                        if (!isPaused) timer.resume();
                    } else if (pauseButton.getGlobalBounds().contains(x, y)) {
                        togglePause();
                    } else if (debugButton.getGlobalBounds().contains(x, y)) {
                        debugMode = !debugMode;
                    } else if (!isPaused && !board.isGameOver() && !board.isVictory()) {
                        board.handleLeftClick(x, y);
                    }
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (!isPaused && !board.isGameOver() && !board.isVictory()) {
                        board.handleRightClick(x, y);
                    }
                }

                updateFace();
            }
        }

        if (board.isVictory() && !victoryHandled) {
            victoryHandled = true;
            timer.pause();
            std::string t = formatTime(timer.getElapsedSeconds());
            openLeaderboard(t, true);
        }

        window.clear(sf::Color::White);
        board.draw(window, debugMode || isPaused); // isPaused일 때도 전체 공개
        window.draw(faceButton);
        window.draw(leaderboardButton);
        window.draw(pauseButton);
        window.draw(debugButton);
        drawFlagCounter(window);
        drawTimer(window);
        window.display();
    }
}