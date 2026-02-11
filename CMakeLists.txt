cmake_minimum_required(VERSION 3.16)
project(TravelBookingApp VERSION 1.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific settings
if(WIN32)
    add_definitions(-DNANA_WINDOWS)
    add_definitions(-D_UNICODE -DUNICODE)
endif()

# Add Nana as a subdirectory
add_subdirectory(lib/nana)

# Collect source files
file(GLOB CORE_SOURCES "src/*.cpp")
file(GLOB GUI_SOURCES "src/gui/*.cpp")

# Remove console main.cpp from core sources (we'll use GUI main)
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*main\\.cpp$")
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*Menu\\.cpp$")

# Create the GUI executable
add_executable(TravelBookingGUI WIN32
    ${CORE_SOURCES}
    ${GUI_SOURCES}
)

# Include directories
target_include_directories(TravelBookingGUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/nana/include
)

# Link Nana library
target_link_libraries(TravelBookingGUI PRIVATE nana)

# Copy data files to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Console version (original)
add_executable(TravelBookingConsole
    ${CORE_SOURCES}
    src/Menu.cpp
    src/main.cpp
)

target_include_directories(TravelBookingConsole PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
