cmake_minimum_required(VERSION 3.16)

project(AdvancedAlgorithmsA2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(advancedAlgorithmsA2
    src/main.cpp
    src/Graph.cpp
    src/StoerWagner.cpp
    src/Karger.cpp
)

target_include_directories(advancedAlgorithmsA2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)