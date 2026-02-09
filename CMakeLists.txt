cmake_minimum_required(VERSION 3.16)
project(QtMusicPlayer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Help CMake find SFML from MSYS2 MINGW64
list(APPEND CMAKE_PREFIX_PATH "C:/msys64/mingw64" "C:/msys64/mingw64/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(SFML 3 REQUIRED COMPONENTS Audio System)

qt_standard_project_setup()

qt_add_executable(QtMusicPlayer
    main.cpp
    mainwindow.h
    mainwindow.cpp
)

target_link_libraries(QtMusicPlayer PRIVATE
    Qt6::Widgets
    SFML::Audio
    SFML::System
)
