/**
 * @file gui_main.cpp
 * @author [Your Name]
 * @date 2026-01-21
 * @brief GUI Entry point for the Travel Booking System
 */

#include "BookingSystem.h"
#include "gui/MainWindow.h"

#ifdef _WIN32
    #include <windows.h>
#endif

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    try {
        // Create booking system (loads data from files)
        BookingSystem system;
        
        // Create and run main window
        MainWindow mainWindow(system);
        mainWindow.run();
        
        // Data is automatically saved when system destructor is called
        
    } catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Fatal Error", MB_ICONERROR);
        return 1;
    }
    
    return 0;
}
