set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk")

cmake_minimum_required(VERSION 3.14)
project(Project3_COP3503C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES main.cpp
        Tile.cpp
        Tile.h
        WelcomeWindow.cpp
        WelcomeWindow.h
        GameWindow.cpp
        GameWindow.h
        Config.cpp
        Config.h
        Board.cpp
        Board.h
        LeaderboardWindow.cpp
        LeaderboardWindow.h
        TextureManager.cpp
        TextureManager.h
        TimerPause.cpp
        TimerPause.h)

add_executable(Project3_COP3503C ${SOURCE_FILES})

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(Project3_COP3503C sfml-system sfml-window sfml-graphics sfml-audio sfml-network)