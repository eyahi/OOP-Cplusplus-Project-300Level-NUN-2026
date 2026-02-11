/**
 * @file Menu.cpp
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Implementation of Menu class - Console UI
 * 
 * Provides interactive menu system for the travel booking application.
 * Includes input validation and error handling.
 */

#include "Menu.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#endif

Menu::Menu(BookingSystem& bookingSystem) : bookingSystem(bookingSystem) {}

void Menu::clearScreen() const {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void Menu::pauseScreen() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Menu::displayHeader(const std::string& title) const {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║" << std::setw(35 + title.length()/2) << title 
              << std::setw(29 - title.length()/2) << "║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

int Menu::getIntInput(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[ERROR] Please enter a number between " << min << " and " << max << ".\n";
    }
}

std::string Menu::getStringInput(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (!input.empty()) {
            return input;
        }
        std::cout << "[ERROR] Input cannot be empty.\n";
    }
}

double Menu::getDoubleInput(const std::string& prompt, double min) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[ERROR] Please enter a valid number >= " << min << ".\n";
    }
}

void Menu::displayMainMenu() const {
    displayHeader("TRAVEL BOOKING SYSTEM");
    std::cout << "\n";
    std::cout << "  +-------------------------------------------+\n";
    std::cout << "  |         Welcome to TravelBook!           |\n";
    std::cout << "  +-------------------------------------------+\n";
    std::cout << "  |  1. Login                                 |\n";
    std::cout << "  |  2. Register                              |\n";
    std::cout << "  |  3. View All Flights                      |\n";
    std::cout << "  |  4. View All Hotels                       |\n";
    std::cout << "  |  0. Exit                                  |\n";
    std::cout << "  +-------------------------------------------+\n";
}

void Menu::displayUserMenu() const {
    User* user = bookingSystem.getCurrentUser();
    displayHeader("TRAVEL BOOKING SYSTEM");
    std::cout << "\n  Welcome back, " << user->getFullName() << "!\n";
    std::cout << "\n";
    std::cout << "  +-------------------------------------------+\n";
    std::cout << "  |              MAIN MENU                    |\n";
    std::cout << "  +-------------------------------------------+\n";
    std::cout << "  |  1. Search & Book Flights                 |\n";
    std::cout << "  |  2. Search & Book Hotels                  |\n";
    std::cout << "  |  3. View My Bookings                      |\n";
    std::cout << "  |  4. Cancel a Booking                      |\n";
    std::cout << "  |  5. View My Profile                       |\n";
    std::cout << "  |  0. Logout                                |\n";
    std::cout << "  +-------------------------------------------+\n";
    
    // Show destination prompt if flight was just booked
    if (!bookingSystem.getLastBookedDestination().empty()) {
        std::cout << "\n  TIP: Hotels available in " << bookingSystem.getLastBookedDestination() << "!\n";
        std::cout << "       Select option 2 to book a hotel at your destination.\n";
    }
}

void Menu::handleRegistration() {
    displayHeader("USER REGISTRATION");
    
    std::string username = getStringInput("  Enter username: ");
    std::string password = getStringInput("  Enter password: ");
    std::string email = getStringInput("  Enter email: ");
    std::string fullName = getStringInput("  Enter full name: ");
    std::string phone = getStringInput("  Enter phone number: ");

    if (bookingSystem.registerUser(username, password, email, fullName, phone)) {
        std::cout << "\n  [SUCCESS] Registration successful! You can now login.\n";
    } else {
        std::cout << "\n  [ERROR] Registration failed. Username may already exist.\n";
    }
}

void Menu::handleLogin() {
    displayHeader("USER LOGIN");
    
    std::string username = getStringInput("  Enter username: ");
    std::string password = getStringInput("  Enter password: ");

    if (bookingSystem.login(username, password)) {
        std::cout << "\n  [SUCCESS] Login successful! Welcome, " << bookingSystem.getCurrentUser()->getFullName() << "!\n";
    } else {
        std::cout << "\n  [ERROR] Login failed. Invalid username or password.\n";
    }
}

void Menu::handleFlightBooking() {
    displayHeader("FLIGHT SEARCH & BOOKING");
    
    std::string origin = getStringInput("  Enter origin city (or press Enter for all): ");
    std::string destination = getStringInput("  Enter destination city (or press Enter for all): ");
    
    // Allow empty strings for searching all
    if (origin == " ") origin = "";
    if (destination == " ") destination = "";

    auto flights = bookingSystem.searchFlights(origin, destination);
    
    if (flights.empty()) {
        std::cout << "\n  [ERROR] No flights found matching your criteria.\n";
        return;
    }

    std::cout << "\n  Found " << flights.size() << " flight(s):\n";
    for (auto* flight : flights) {
        flight->displayInfo();
    }

    std::cout << "\n  Would you like to book a flight? (Enter Flight ID or 0 to cancel): ";
    int flightId = getIntInput("", 0, 9999);
    
    if (flightId == 0) return;

    Flight* selectedFlight = bookingSystem.findFlightById(flightId);
    if (!selectedFlight) {
        std::cout << "\n  [ERROR] Invalid flight ID.\n";
        return;
    }

    int numPassengers = getIntInput("  Number of passengers: ", 1, selectedFlight->getAvailableSeats());
    
    std::cout << "\n  Select seat class:\n";
    std::cout << "    1. Economy (1x price)\n";
    std::cout << "    2. Business (2x price)\n";
    std::cout << "    3. First Class (3x price)\n";
    int classChoice = getIntInput("  Your choice: ", 1, 3);
    
    std::string seatClass;
    switch (classChoice) {
        case 1: seatClass = "Economy"; break;
        case 2: seatClass = "Business"; break;
        case 3: seatClass = "First"; break;
    }

    std::string passengerName = getStringInput("  Primary passenger name: ");

    if (bookingSystem.bookFlight(flightId, numPassengers, seatClass, passengerName)) {
        std::cout << "\n  [SUCCESS] Flight booked successfully!\n";
        std::cout << "\n  Would you like to book a hotel in " << bookingSystem.getLastBookedDestination() << "?\n";
        std::cout << "     (Enter 1 for Yes, 0 for No): ";
        int hotelChoice = getIntInput("", 0, 1);
        if (hotelChoice == 1) {
            handleHotelAfterFlight();
        }
    } else {
        std::cout << "\n  [ERROR] Booking failed. Please try again.\n";
    }
}

void Menu::handleHotelAfterFlight() {
    std::string destination = bookingSystem.getLastBookedDestination();
    
    displayHeader("HOTELS IN " + destination);
    
    auto hotels = bookingSystem.getHotelsInDestination();
    
    if (hotels.empty()) {
        std::cout << "\n  [ERROR] No hotels available in " << destination << ".\n";
        return;
    }

    std::cout << "\n  Found " << hotels.size() << " hotel(s) in " << destination << ":\n";
    for (auto* hotel : hotels) {
        hotel->displayInfo();
    }

    std::cout << "\n  Would you like to book a hotel? (Enter Hotel ID or 0 to cancel): ";
    int hotelId = getIntInput("", 0, 9999);
    
    if (hotelId == 0) return;

    Hotel* selectedHotel = bookingSystem.findHotelById(hotelId);
    if (!selectedHotel || selectedHotel->getCity() != destination) {
        std::cout << "\n  [ERROR] Invalid hotel ID or hotel not in " << destination << ".\n";
        return;
    }

    std::string checkIn = getStringInput("  Check-in date (YYYY-MM-DD): ");
    std::string checkOut = getStringInput("  Check-out date (YYYY-MM-DD): ");
    int numRooms = getIntInput("  Number of rooms: ", 1, selectedHotel->getAvailableRooms());
    int numGuests = getIntInput("  Number of guests: ", 1, numRooms * 4);
    std::string guestName = getStringInput("  Primary guest name: ");

    if (bookingSystem.bookHotel(hotelId, checkIn, checkOut, numRooms, numGuests, guestName)) {
        std::cout << "\n  [SUCCESS] Hotel booked successfully!\n";
    } else {
        std::cout << "\n  [ERROR] Booking failed. Please try again.\n";
    }
}

void Menu::handleHotelBooking() {
    // Check if there's a destination from a recent flight booking
    if (!bookingSystem.getLastBookedDestination().empty()) {
        std::cout << "\n  You recently booked a flight to " << bookingSystem.getLastBookedDestination() << ".\n";
        std::cout << "     Would you like to see hotels there? (1 = Yes, 0 = No): ";
        int choice = getIntInput("", 0, 1);
        if (choice == 1) {
            handleHotelAfterFlight();
            return;
        }
    }

    displayHeader("HOTEL SEARCH & BOOKING");
    
    std::string city = getStringInput("  Enter city name (or press Enter for all): ");
    if (city == " ") city = "";

    auto hotels = bookingSystem.searchHotels(city);
    
    if (hotels.empty()) {
        std::cout << "\n  [ERROR] No hotels found matching your criteria.\n";
        return;
    }

    std::cout << "\n  Found " << hotels.size() << " hotel(s):\n";
    for (auto* hotel : hotels) {
        hotel->displayInfo();
    }

    std::cout << "\n  Would you like to book a hotel? (Enter Hotel ID or 0 to cancel): ";
    int hotelId = getIntInput("", 0, 9999);
    
    if (hotelId == 0) return;

    Hotel* selectedHotel = bookingSystem.findHotelById(hotelId);
    if (!selectedHotel) {
        std::cout << "\n  [ERROR] Invalid hotel ID.\n";
        return;
    }

    std::string checkIn = getStringInput("  Check-in date (YYYY-MM-DD): ");
    std::string checkOut = getStringInput("  Check-out date (YYYY-MM-DD): ");
    int numRooms = getIntInput("  Number of rooms: ", 1, selectedHotel->getAvailableRooms());
    int numGuests = getIntInput("  Number of guests: ", 1, numRooms * 4);
    std::string guestName = getStringInput("  Primary guest name: ");

    if (bookingSystem.bookHotel(hotelId, checkIn, checkOut, numRooms, numGuests, guestName)) {
        std::cout << "\n  [SUCCESS] Hotel booked successfully!\n";
    } else {
        std::cout << "\n  [ERROR] Booking failed. Please try again.\n";
    }
}

void Menu::viewMyBookings() {
    displayHeader("MY BOOKINGS");
    
    auto bookings = bookingSystem.getUserBookings();
    
    if (bookings.empty()) {
        std::cout << "\n  You have no bookings yet.\n";
        return;
    }

    std::cout << "\n  You have " << bookings.size() << " booking(s):\n";
    for (auto* booking : bookings) {
        booking->displayBooking();
    }
}

void Menu::handleCancellation() {
    displayHeader("CANCEL BOOKING");
    
    auto bookings = bookingSystem.getUserBookings();
    
    if (bookings.empty()) {
        std::cout << "\n  You have no bookings to cancel.\n";
        return;
    }

    std::cout << "\n  Your active bookings:\n";
    for (auto* booking : bookings) {
        if (booking->getStatus() != BookingStatus::CANCELLED) {
            booking->displayBooking();
        }
    }

    int bookingId = getIntInput("\n  Enter Booking ID to cancel (0 to go back): ", 0, 9999);
    
    if (bookingId == 0) return;

    if (bookingSystem.cancelBooking(bookingId)) {
        std::cout << "\n  [SUCCESS] Booking cancelled successfully!\n";
    } else {
        std::cout << "\n  [ERROR] Cancellation failed. Invalid booking ID or already cancelled.\n";
    }
}

void Menu::viewProfile() {
    User* user = bookingSystem.getCurrentUser();
    if (user) {
        user->displayProfile();
    }
}

void Menu::run() {
    std::cout << "\n  Loading Travel Booking System...\n";
    
    int choice;
    
    while (true) {
        if (bookingSystem.isLoggedIn()) {
            displayUserMenu();
            choice = getIntInput("\n  Enter your choice: ", 0, 5);
            
            switch (choice) {
                case 1:
                    handleFlightBooking();
                    break;
                case 2:
                    handleHotelBooking();
                    break;
                case 3:
                    viewMyBookings();
                    break;
                case 4:
                    handleCancellation();
                    break;
                case 5:
                    viewProfile();
                    break;
                case 0:
                    bookingSystem.logout();
                    std::cout << "\n  Logged out successfully!\n";
                    break;
            }
        } else {
            displayMainMenu();
            choice = getIntInput("\n  Enter your choice: ", 0, 4);
            
            switch (choice) {
                case 1:
                    handleLogin();
                    break;
                case 2:
                    handleRegistration();
                    break;
                case 3:
                    bookingSystem.displayAllFlights();
                    break;
                case 4:
                    bookingSystem.displayAllHotels();
                    break;
                case 0:
                    std::cout << "\n  +==============================================================+\n";
                    std::cout << "  |     Thank you for using TravelBook! Safe travels!           |\n";
                    std::cout << "  +==============================================================+\n\n";
                    return;
            }
        }
        
        if (choice != 0) {
            pauseScreen();
        }
    }
}
