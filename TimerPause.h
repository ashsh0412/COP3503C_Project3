#pragma once
#include <ctime>

class TimerPause {
private:
    std::time_t startTime;
    int elapsedSeconds;
    bool paused;

public:
    TimerPause();
    void start();
    void pause();
    void resume();
    void reset();
    int getElapsedSeconds();
};