#pragma once
#include <string>

class Config {
    int cols, rows, mines;
public:
    Config(const std::string& path);
    int getCols() const;
    int getRows() const;
    int getMines() const;
};