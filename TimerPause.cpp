#include "TimerPause.h"
using namespace std;

// https://cplusplus.com/reference/ctime/

TimerPause::TimerPause() {
    elapsedSeconds = 0;
    paused = false;
    startTime = 0;
}

void TimerPause::start() {
    elapsedSeconds = 0;
    paused = false;
    startTime = time(nullptr);
}

void TimerPause::pause() {
    if (!paused) {
        paused = true;
        time_t now = time(nullptr);
        elapsedSeconds += (int)(now - startTime);
    }
}

void TimerPause::resume() {
    if (paused) {
        paused = false;
        startTime = time(nullptr);
    }
}

void TimerPause::reset() {
    paused = false;
    elapsedSeconds = 0;
    startTime = time(nullptr);
}

int TimerPause::getElapsedSeconds() {
    if (paused) {
        return elapsedSeconds;
    } else {
        time_t now = time(nullptr);
        return elapsedSeconds + (int)(now - startTime);
    }
}