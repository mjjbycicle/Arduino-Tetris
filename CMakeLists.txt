cmake_minimum_required(VERSION 3.27)
project(Tetris)

set(CMAKE_CXX_STANDARD 23)

add_executable(Tetris main.cpp
        Block.h
        display.h
)
