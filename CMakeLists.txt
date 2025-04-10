cmake_minimum_required(VERSION 3.14)
project(Project3_COP3503C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

add_executable(Project3_COP3503C ${SOURCE_FILES})

# SFML 설치 경로 명시 (sfml@2 버전일 경우)
set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(Project3_COP3503C sfml-system sfml-window sfml-graphics sfml-audio sfml-network)