cmake_minimum_required(VERSION 3.16)

project(PulseChat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ONE find_package only
find_package(Qt6 REQUIRED COMPONENTS Widgets Network Multimedia)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    loginwindow.cpp
    loginwindow.h
    loginwindow.ui

    signupwindow.cpp
    signupwindow.h
    signupwindow.ui

    resources.qrc
)

qt_add_executable(PulseChat
    ${PROJECT_SOURCES}
)

target_link_libraries(PulseChat
    PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
)


# Windows settings
set_target_properties(PulseChat PROPERTIES
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PulseChat)
endif()
