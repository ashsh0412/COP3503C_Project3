#include "GameWindow.h"
#include "LeaderboardWindow.h"
using namespace std;

GameWindow::GameWindow(Config& config, const string& name) : config(config), board(config.getRows(), config.getCols(), config.getMines()), playerName(name) {
    loadTextures();
    setupTiles();
    timer.start();
}

void GameWindow::loadTextures() {
    textures.load("tile_hidden", "files/images/tile_hidden.png");
    textures.load("tile_revealed", "files/images/tile_revealed.png");
    textures.load("flag", "files/images/flag.png");
    textures.load("mine", "files/images/mine.png");
    for (int i = 1; i <= 8; ++i) {
        textures.load("number_" + to_string(i), "files/images/number_" + to_string(i) + ".png");
    }
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
            tile.setRevealedTexture(*textures.get("tile_revealed"));

            for (int i = 1; i <= 8; ++i) {
                tile.setNumberTexture(i, *textures.get("number_" + to_string(i)));
            }
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
    int totalElapsedSeconds = timer.getElapsedSeconds();

    int minutes = totalElapsedSeconds / 60;
    int seconds = totalElapsedSeconds % 60;

    // getting number pic
    sf::Texture* digitImage = textures.get("digits");

    int baseX = config.getCols() * 32 - 97;
    int baseY = config.getRows() * 32 + 48;

    auto drawOneDigit = [&](int digit, int offsetX) {
        sf::Sprite digitSprite;
        digitSprite.setTexture(*digitImage);

        // https://www.sfml-dev.org/tutorials/2.5/graphics-sprite.php, 6.2 images
        sf::IntRect partOfImage(digit * 21, 0, 21, 32);
        digitSprite.setTextureRect(partOfImage);

        // set position
        digitSprite.setPosition((float)(baseX + offsetX), (float)baseY);

        window.draw(digitSprite);
    };

    drawOneDigit(minutes / 10, 0);
    drawOneDigit(minutes % 10, 21);
    drawOneDigit(seconds / 10, 21 * 2 + 12);
    drawOneDigit(seconds % 10, 21 * 3 + 12);
}

void GameWindow::drawFlagCounter(sf::RenderWindow& window) {
    int mines = config.getMines() - board.getFlagCount();
    sf::Texture* digits = textures.get("digits");

    int x = 33;
    int y = config.getRows() * 32 + 48;

    // negative counts
    if (mines < 0) {
        sf::Sprite dash;
        dash.setTexture(*digits);
        dash.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
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
// ex) 70s -> 1:10
string GameWindow::formatTime(int sec) const {
    int m = sec / 60;
    int s = sec % 60;

    string result;
    if (m < 10) result += "0";
    result += to_string(m);
    result += ":";

    if (s < 10) result += "0";
    result += to_string(s);

    return result;
}

void GameWindow::openLeaderboard(const string& time, bool win) {
    LeaderboardWindow(config, win ? playerName : "", win ? time : "").run();
}

void GameWindow::run() {

    // begin the game
    int width = config.getCols() * 32;
    int height = config.getRows() * 32 + 100;

    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

    loadTextures();
    setupTiles();
    timer.start();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                // get click's position
                float x = static_cast<float>(sf::Mouse::getPosition(window).x);
                float y = static_cast<float>(sf::Mouse::getPosition(window).y);

                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (faceButton.getGlobalBounds().contains(x, y)) {
                        board.reset();
                        setupTiles();
                        timer.reset();
                        timer.start();
                        victoryHandled = false;
                        isPaused = false;
                        pauseButton.setTexture(*textures.get("pause"));
                    }
                    else if (leaderboardButton.getGlobalBounds().contains(x, y)) {
                        bool wasPaused = isPaused;
                        bool wasDebug = debugMode;

                        isPaused = true;
                        timer.pause();

                        window.clear(sf::Color::White);
                        board.draw(window, /* revealPaused = */ true, /* revealMines = */ false);
                        window.draw(faceButton);
                        window.draw(leaderboardButton);
                        window.draw(pauseButton);
                        window.draw(debugButton);
                        drawFlagCounter(window);
                        drawTimer(window);
                        window.display();

                        openLeaderboard();

                        isPaused = wasPaused;
                        debugMode = wasDebug;

                        if (!isPaused && !board.isVictory() && !board.isGameOver())
                            timer.resume();
                    }
                    else if (pauseButton.getGlobalBounds().contains(x, y)) {
                        if (!board.isVictory() && !board.isGameOver()) {
                            togglePause();
                        }
                    }
                    else if (debugButton.getGlobalBounds().contains(x, y)) {
                        if (!board.isVictory() && !board.isGameOver()) {
                            debugMode = !debugMode;
                        }
                    }
                    else if (!isPaused && !board.isGameOver() && !board.isVictory()) {
                        // handleLeftClick --> board, reveal tile
                        board.handleLeftClick(x, y);
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (!isPaused && !board.isGameOver() && !board.isVictory()) {
                        // handleRightClick --> board, flag or unflag
                        board.handleRightClick(x, y);
                    }
                }

                updateFace();
            }
        }

        window.clear(sf::Color::White);
        board.draw(window, /* revealPaused = */ isPaused, /* revealMines = */ debugMode && !isPaused);
        window.draw(faceButton);
        window.draw(leaderboardButton);
        window.draw(pauseButton);
        window.draw(debugButton);
        drawFlagCounter(window);
        drawTimer(window);
        window.display();

        if (board.isVictory() && !victoryHandled) {
            // make sure function once
            victoryHandled = true;
            timer.pause();
            string time = formatTime(timer.getElapsedSeconds());
            openLeaderboard(time, true);
        }
    }
}