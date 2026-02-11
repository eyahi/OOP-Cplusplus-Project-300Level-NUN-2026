/**
 * @file Menu.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Console menu system for user interaction
 * 
 * This file defines the Menu class which provides a console-based
 * user interface for the travel booking system.
 */

#ifndef MENU_H
#define MENU_H

#include "BookingSystem.h"
#include <string>

/**
 * @class Menu
 * @brief Console-based menu interface
 * 
 * Handles all user interactions through a menu-driven console interface.
 * Includes input validation and error handling.
 */
class Menu {
private:
    BookingSystem& bookingSystem;  ///< Reference to the booking system

    /**
     * @brief Display the main menu options
     */
    void displayMainMenu() const;

    /**
     * @brief Display the logged-in user menu
     */
    void displayUserMenu() const;

    /**
     * @brief Handle user registration
     */
    void handleRegistration();

    /**
     * @brief Handle user login
     */
    void handleLogin();

    /**
     * @brief Handle flight search and booking
     */
    void handleFlightBooking();

    /**
     * @brief Handle hotel search and booking
     */
    void handleHotelBooking();

    /**
     * @brief Handle hotel booking after flight (destination-aware)
     */
    void handleHotelAfterFlight();

    /**
     * @brief View user's bookings
     */
    void viewMyBookings();

    /**
     * @brief Handle booking cancellation
     */
    void handleCancellation();

    /**
     * @brief Display user profile
     */
    void viewProfile();

    /**
     * @brief Get validated integer input
     * @param prompt Prompt message
     * @param min Minimum valid value
     * @param max Maximum valid value
     * @return Validated integer
     */
    int getIntInput(const std::string& prompt, int min, int max);

    /**
     * @brief Get validated string input
     * @param prompt Prompt message
     * @return Non-empty string
     */
    std::string getStringInput(const std::string& prompt);

    /**
     * @brief Get validated double input
     * @param prompt Prompt message
     * @param min Minimum valid value
     * @return Validated double
     */
    double getDoubleInput(const std::string& prompt, double min);

    /**
     * @brief Clear the console screen
     */
    void clearScreen() const;

    /**
     * @brief Pause and wait for user input
     */
    void pauseScreen() const;

    /**
     * @brief Display a header banner
     * @param title Title text
     */
    void displayHeader(const std::string& title) const;

public:
    /**
     * @brief Construct a new Menu object
     * @param system Reference to the booking system
     */
    explicit Menu(BookingSystem& bookingSystem);

    /**
     * @brief Run the main menu loop
     */
    void run();
};

#endif // MENU_H
