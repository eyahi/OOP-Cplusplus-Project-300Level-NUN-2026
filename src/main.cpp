/**
 * @file main.cpp
 * @author  Muhammad Tasiu
 * @date 2026-01-21
 * @brief Entry point for the Travel Booking System
 * 
 * This file contains the main function that initializes the booking
 * system and runs the interactive menu.
 * 
 * Course: Object-Oriented Programming with C++
 * Assignment: Travel Booking Application
 */

#include "BookingSystem.h"
#include "Menu.h"
#include <iostream>
#include <locale>

#ifdef _WIN32
    #include <windows.h>
#endif

/**
 * @brief Main entry point
 * @return Exit code (0 for success)
 */
int main() {
    // Enable Unicode output on Windows
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    try {
        // Create booking system (loads data from files)
        BookingSystem system;
        
        // Create and run menu
        Menu menu(system);
        menu.run();
        
        // Data is automatically saved when system destructor is called
        
    } catch (const std::exception& e) {
        std::cerr << "[FATAL ERROR] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
