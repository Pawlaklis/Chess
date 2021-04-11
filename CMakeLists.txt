cmake_minimum_required(VERSION 3.17)
project(SFML_game)

set(CMAKE_CXX_STANDARD 14)
# SFML
## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "./library/frameworks/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
add_executable(sfml main.cpp src/Board.cpp src/Board.h src/piece.cpp src/piece.h)
target_link_libraries(sfml sfml-system sfml-window sfml-graphics sfml-network sfml-audio)