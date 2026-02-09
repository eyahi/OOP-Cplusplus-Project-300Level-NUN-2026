cmake_minimum_required(VERSION 3.16)
project(TodoListManager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "Todo List Manager"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.todolist.manager"
    )
    
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    
    find_program(QMAKE_EXECUTABLE qmake)
    if(QMAKE_EXECUTABLE)
        execute_process(
            COMMAND ${QMAKE_EXECUTABLE} -query QT_INSTALL_LIBS
            OUTPUT_VARIABLE QT_LIB_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()
    
    if(NOT QT_LIB_DIR OR QT_LIB_DIR STREQUAL "")
        set(QT_LIB_DIR "/opt/homebrew/lib")
    endif()
    
    message(STATUS "Qt library directory: ${QT_LIB_DIR}")
endif()
