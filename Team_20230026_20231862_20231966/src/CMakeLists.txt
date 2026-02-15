cmake_minimum_required(VERSION 4.0)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src
        Board.cpp
        Controller.cpp
        GridView.cpp
        main.cpp
        MainWindow.cpp
        Solver.cpp)
