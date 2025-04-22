#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>

class TimerPause {
private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point pausedTime;
    bool paused = false;
    int elapsedWhenPaused = 0;

public:
    void start();
    void pause();
    void resume();
    void reset();
    int getElapsedSeconds() const;
};