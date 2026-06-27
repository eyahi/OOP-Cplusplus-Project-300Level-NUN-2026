cmake_minimum_required(VERSION 3.16)

#     FORCE MINGW GENERATOR AND MAKE TOOL
set(CMAKE_GENERATOR "MinGW Makefiles" CACHE INTERNAL "" FORCE)
set(CMAKE_MAKE_PROGRAM "C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe" CACHE INTERNAL "" FORCE)

# FORCE THE COMPILERS
set(CMAKE_CXX_COMPILER "C:/Qt/Tools/mingw1310_64/bin/g++.exe")

# POINT TO THE QT LIBRARIES
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/mingw_64")

project(DiaryApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
include_directories(include)

add_executable(DiaryApp 
    src/main.cpp 
    src/DiaryApp.cpp 
    include/DiaryApp.h
)

target_link_libraries(DiaryApp PRIVATE Qt6::Widgets)