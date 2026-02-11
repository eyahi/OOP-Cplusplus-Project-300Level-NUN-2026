cmake_minimum_required(VERSION 3.16)

project(FlightBookingSystem)

set(CMAKE_CXX_STANDARD 17)

# Find Qt6 Widgets module
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Add the source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/BookingSystem.cpp
    src/Flight.cpp
)

# Add the headers with Q_OBJECT so MOC runs correctly
set(HEADERS
    src/MainWindow.h
)

# Create executable
add_executable(FlightBookingSystem
    ${SOURCES}
    ${HEADERS}
)

# Tell CMake to run MOC automatically
qt6_wrap_cpp(MOCED_HEADERS ${HEADERS})
target_sources(FlightBookingSystem PRIVATE ${MOCED_HEADERS})

# Link Qt6 Widgets
target_link_libraries(FlightBookingSystem Qt6::Widgets)
