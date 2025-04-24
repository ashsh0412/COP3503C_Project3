#include "Config.h"
#include <fstream>
#include <stdexcept>
using namespace std;
Config::Config(const string& path) {
    ifstream file(path);
    if (!file.is_open()) throw runtime_error("Cannot open config file.");
    if (!(file >> cols >> rows >> mines)) throw runtime_error("Invalid config format.");
    if (cols <= 0 || rows <= 0 || mines < 0 || mines > cols * rows)
        throw runtime_error("Invalid board parameters.");
}

int Config::getCols() const { return cols; }
int Config::getRows() const { return rows; }
int Config::getMines() const { return mines; }