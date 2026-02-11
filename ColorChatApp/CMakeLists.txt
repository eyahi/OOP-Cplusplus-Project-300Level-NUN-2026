cmake_minimum_required(VERSION 3.20)

project(ColorfulChatApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Prefer Qt6, fallback to Qt5
find_package(Qt6 QUIET COMPONENTS Core Network Widgets)
if (Qt6_FOUND)
    set(QT Qt6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Core Network Widgets)
    set(QT Qt5)
endif()
# ---- Build quality options (warnings + optional sanitizers) ----
option(ENABLE_WARNINGS "Enable extra compiler warnings" ON)
option(ENABLE_ASAN "Enable AddressSanitizer (Debug recommended)" OFF)

if (ENABLE_WARNINGS)
  if (MSVC)
    add_compile_options(/W4 /permissive-)
  else()
    add_compile_options(-Wall -Wextra -Wpedantic)
  endif()
endif()

if (ENABLE_ASAN AND NOT MSVC)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address)
endif()


add_library(chatcore
    Codes/Client/chatclient.cpp
    Codes/Client/chatmessage.cpp
    Codes/Client/userprofile.cpp
    Codes/Client/chatclient.h
    Codes/Client/chatmessage.h
    Codes/Client/userprofile.h
)
target_include_directories(chatcore PUBLIC Codes/Client)
target_link_libraries(chatcore PUBLIC ${QT}::Core ${QT}::Network)

add_executable(ChatServer
    Codes/Server/main.cpp
    Codes/Server/ChatServer.cpp
    Codes/Server/ChatServer.h
    Codes/Server/chatroom.h
    Codes/Server/message.h
    Codes/Server/user.h
)
target_include_directories(ChatServer PRIVATE Codes/Server)
target_link_libraries(ChatServer PRIVATE ${QT}::Core ${QT}::Network)

add_executable(ColorChatClient WIN32
    gui/main.cpp
    gui/mainwindow.cpp
    gui/mainwindow.h
    gui/bubblewidget.cpp
    gui/bubblewidget.h
    gui/thememanager.cpp
    gui/thememanager.h
    gui/connectdialog.cpp
    gui/connectdialog.h
)
target_include_directories(ColorChatClient PRIVATE gui Codes/Client)
target_link_libraries(ColorChatClient PRIVATE chatcore ${QT}::Core ${QT}::Network ${QT}::Widgets)

# On macOS, building a .app bundle makes running from terminal slightly different.
# Keep it OFF by default so "./build/ColorChatClient" works in VS Code terminal.
option(BUILD_MACOS_BUNDLE "Build ColorChatClient as a macOS .app bundle" OFF)
if (APPLE AND BUILD_MACOS_BUNDLE)
    set_target_properties(ColorChatClient PROPERTIES MACOSX_BUNDLE TRUE)
endif()
