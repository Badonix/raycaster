cmake_minimum_required(VERSION 3.20)
project(Raycaster CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

set(SOURCES
    src/main.cpp
    src/game.cpp
    src/player.cpp
    src/sdlrenderer.cpp
    src/texture.cpp
)

add_executable(Raycaster ${SOURCES})

target_include_directories(Raycaster PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Raycaster PRIVATE SDL2::SDL2)
