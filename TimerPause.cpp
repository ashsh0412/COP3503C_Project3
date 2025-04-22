#include "TimerPause.h"

void TimerPause::start() {
    paused = false;
    elapsedWhenPaused = 0;
    startTime = std::chrono::steady_clock::now();
}

void TimerPause::pause() {
    if (!paused) {
        paused = true;
        pausedTime = std::chrono::steady_clock::now();
        elapsedWhenPaused += std::chrono::duration_cast<std::chrono::seconds>(pausedTime - startTime).count();
    }
}

void TimerPause::resume() {
    if (paused) {
        paused = false;
        startTime = std::chrono::steady_clock::now();
    }
}

void TimerPause::reset() {
    startTime = std::chrono::steady_clock::now();
    elapsedWhenPaused = 0;
    paused = false;
}

int TimerPause::getElapsedSeconds() const {
    if (paused) {
        return elapsedWhenPaused;
    } else {
        auto now = std::chrono::steady_clock::now();
        return elapsedWhenPaused + std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
    }
}