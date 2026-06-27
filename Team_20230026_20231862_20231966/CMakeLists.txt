cmake_minimum_required(VERSION 3.20)

project(SudokuGUI LANGUAGES CXX)

add_executable(SudokuGUI WIN32
    src/main.cpp
    src/MainWindow.cpp
    src/GridView.cpp
    src/Controller.cpp
    src/Board.cpp
    src/Solver.cpp
)

target_include_directories(SudokuGUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(SudokuGUI PRIVATE cxx_std_14)

if (MSVC)
    target_compile_options(SudokuGUI PRIVATE /W4 /permissive-)
else()
    target_compile_options(SudokuGUI PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_definitions(SudokuGUI PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)

target_link_libraries(SudokuGUI PRIVATE comctl32)

